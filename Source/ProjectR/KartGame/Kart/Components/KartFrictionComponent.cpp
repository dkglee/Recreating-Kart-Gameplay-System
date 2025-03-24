// Fill out your copyright notice in the Description page of Project Settings.


#include "KartFrictionComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "InputAction.h"
#include "Kart.h"
#include "KartSteeringComponent.h"
#include "KartSuspensionComponent.h"
#include "KartSystemLibrary.h"
#include "StringDev.h"


// Sets default values for this component's properties
UKartFrictionComponent::UKartFrictionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	
	FrictionRollbackTimeline
		= CreateDefaultSubobject<UTimelineComponent>("Friction Rollback Timeline");
	
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_DRIFT
	(TEXT("/Game/Kart/Input/InputAction/IA_KartDrift.IA_KartDrift"));
	if (IA_DRIFT.Succeeded())
	{
		IA_Drift = IA_DRIFT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> C_FRICTIONCURVE
	(TEXT("/Game/Kart/Curves/FrictionCurve.FrictionCurve"));
	if (C_FRICTIONCURVE.Succeeded())
	{
		FrictionCurve = C_FRICTIONCURVE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> C_FRICTIONROLLBACKCURVE
	(TEXT("/Game/Kart/Curves/FrictionRollbackCurve.FrictionRollbackCurve"));
	if (C_FRICTIONCURVE.Succeeded())
	{
		FrictionRollbackCurve = C_FRICTIONCURVE.Object;
	}
}


void UKartFrictionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	FrictionRollbackCallback.BindDynamic(this, &ThisClass::OnFrictionRollbackCallback);
	FrictionRollbackFinish.BindDynamic(this, &ThisClass::OnFrictionRollbackFinish);
	
	FrictionRollbackTimeline->SetLooping(false);
	FrictionRollbackTimeline->AddInterpFloat(FrictionRollbackCurve, FrictionRollbackCallback);
	FrictionRollbackTimeline->SetTimelineFinishedFunc(FrictionRollbackFinish);
}


void UKartFrictionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UKartFrictionComponent::ProcessFriction()
{
	// 입력과 현재 상태에 따라 드리프트를 적용할 지 말지 정함
	DetermineDriftState();
	// 드리프트 상태에 따라 마찰력을 적용
	SetAngularDampling();
	ApplyFrictionToKart_Implementation(bDrift);
	BroadCastDriftEnd();
}

void UKartFrictionComponent::OnDriftInputDetected(const FInputActionValue& InputActionValue)
{
	bDriftInput = InputActionValue.Get<bool>();
}

void UKartFrictionComponent::SetAngularDampling()
{
	// 전방 속도 가져오기
	float NormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());

	// Drift Button Input Detected & Steering Input Detected
	float SteeringInput = Kart->GetSteeringComponent()->GetTargetSteering();
	bool bSteering = !FMath::IsNearlyZero(FMath::Abs(SteeringInput));
	if (bDriftInput && bSteering && bDrift)
	{
		// 드리프트 버튼이 계속 눌리고 있고 방향키 입력도 강하게 눌리고 있을 경우
		KartBody->SetAngularDamping(HardDriftAngularDamping);
		float Threshold = 0.5f;
		if (NormalizedSpeed < Threshold)
		{
			KartBody->SetAngularDamping(DefaultAngularDamping * 0.5f);
		}
	}
	else if (!bDriftInput && bSteering && bDrift)
	{
		// 드리프트 버튼이 안눌리고 방향키 입력만으로 드리프트를 유지하고 있는 경우
		KartBody->SetAngularDamping(NormalAngularDamping);
		float Threshold = 0.5f;
		if (NormalizedSpeed < Threshold)
		{
			KartBody->SetAngularDamping(DefaultAngularDamping * 1.0f);
		}
	}
	else if (!bDriftInput && !bSteering)
	{
		// 드리프트 버튼이 안눌리고 방향키 입력도 없는 경우
		KartBody->SetAngularDamping(DefaultAngularDamping);
	}
	else
	{
		KartBody->SetAngularDamping(DefaultAngularDamping);
	}

}

void UKartFrictionComponent::DetermineDriftState()
{
	// 속도를 구함
	float NormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());
	float NormalizedRightSpeed = UKartSystemLibrary::CalculateNormalizedRightSpeedWithKart(KartBody, Kart->GetMaxSpeed());
	float TotalNormalizedSpeed = NormalizedSpeed + NormalizedRightSpeed;
	bool bSteering = !FMath::IsNearlyZero(FMath::Abs(Kart->GetSteeringComponent()->GetTargetSteering()));

	// 속도가 일정 이상이다. 이전에 드리프트 상태가 아니었다면 드리프트 상태로 변경
	constexpr float Threshold = 0.35f; 
	// bool bFlag = NormalizedSpeed > Threshold;
	bool bFlag = TotalNormalizedSpeed > Threshold;
	if (bFlag && !bDrift && bDriftInput && bSteering)
	{
		// 드리프트 상태로 변경 후 리턴
		bDrift = true;
		return ;
	}

	if (bDrift)
	{
		FVector RightVector = KartBody->GetRightVector();
		FVector LinearVelocity = KartBody->GetPhysicsLinearVelocity();
		float Velocity = FVector::DotProduct(RightVector, LinearVelocity);

		// 오른쪽으로 가는 속도가 있다면 드리프트 상태로 유지
		constexpr float DriftThreshold = 1100.0f;
	
		// slip angle을 구한 다음에 이를 이용하여 드리프트 상태를 결정 -> 사전에 그러면 바퀴가 적절히 돌아가야 함
		FVector WheelForwardVector = Kart->GetLF_Wheel()->GetForwardVector();
		float WheelForwardVelocity = FVector::DotProduct(WheelForwardVector, LinearVelocity);
		FVector WheelRightVector = Kart->GetLF_Wheel()->GetRightVector();
		float WheelRightVelocity = FVector::DotProduct(WheelRightVector, LinearVelocity);
		
		FRotator WheelRot = Kart->GetLF_Wheel()->GetRelativeRotation();
		float SteeringAngle = FMath::DegreesToRadians(WheelRot.Yaw);
		
		float SlipAngle = FMath::Atan2(WheelRightVelocity, WheelForwardVelocity) - SteeringAngle;
		
		bDrift = FMath::Abs(FMath::RadiansToDegrees(SlipAngle)) > 25.0f;
		bDrift |= (FMath::Abs(FMath::RadiansToDegrees(SlipAngle)) > 2.0f && FMath::Abs(Velocity) > DriftThreshold);
		
		bDrift = bDrift || (bDriftInput && bSteering);
		bDrift = bDrift && bFlag;
	}
}

void UKartFrictionComponent::BroadCastDriftEnd()
{
	if (bPrevDrift && !bDrift)
	{
		OnDriftEnded.Broadcast();
	}
	bPrevDrift = bDrift;
}

void UKartFrictionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartFrictionComponent::SetupInputBinding);
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
	}
}

void UKartFrictionComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Drift, ETriggerEvent::Completed, this, &UKartFrictionComponent::OnDriftInputDetected);
	PlayerInputComponent->BindAction(IA_Drift, ETriggerEvent::Triggered, this, &UKartFrictionComponent::OnDriftInputDetected);
}

// 마찰력 적용
void UKartFrictionComponent::ApplyFrictionToKart_Implementation(bool bInDrift)
{
	// 드리프트 상태와 아닌 상태에 따라 마찰력을 적용
	FVector RightVector = KartBody->GetRightVector();
	FVector LinearVelocity = KartBody->GetPhysicsLinearVelocity();
	float Velocity = FVector::DotProduct(RightVector, LinearVelocity);

	float NormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());

	// 예시 변수들
	float TargetFrictionData = bInDrift ? FrictionCurve->GetFloatValue(NormalizedSpeed) : 1.0f;
	float TargetInFrictionGrip = bInDrift ? 0.7f : FrictionGrip;

	// 보간 속도 계수 (너무 높으면 여전히 급격하고, 너무 낮으면 느리게 반응)
	float InterpSpeed = 3.0f; // 적절히 조절

	// Lerp 또는 InterpTo를 활용한 부드러운 전환
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	if (bDrift)
	{
		InFrictionData = TargetFrictionData;
		InFrictionGripCoeff = TargetInFrictionGrip;
	}
	else
	{
		InFrictionData = FMath::FInterpTo(InFrictionData, TargetFrictionData, DeltaTime, InterpSpeed);
		InFrictionGripCoeff = FMath::FInterpTo(InFrictionGripCoeff, TargetInFrictionGrip, DeltaTime, InterpSpeed);
	}
	
	FVector FrictionForce = RightVector * Velocity * -1.5f * InFrictionData * InFrictionGripCoeff;

	KartBody->AddForce(FrictionForce, NAME_None, true);
}

void UKartFrictionComponent::OnFrictionRollbackCallback(const float Value)
{
}

void UKartFrictionComponent::OnFrictionRollbackFinish()
{
}

void UKartFrictionComponent::RollbackFriction()
{
	FrictionRollbackTimeline->PlayFromStart();
}
