// Fill out your copyright notice in the Description page of Project Settings.


#include "KartFrictionComponent.h"

#include "EnhancedInputComponent.h"
#include "FastLogger.h"
#include "Components/BoxComponent.h"
#include "InputAction.h"
#include "Kart.h"
#include "KartSteeringComponent.h"
#include "KartSuspensionComponent.h"
#include "KartSystemLibrary.h"
#include "KartGame/Items/Components/ItemInteractionComponent.h"


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
	UpdateBoosterGauge();
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
		GetWorld()->GetTimerManager().ClearTimer(FrictionDelayTimer);
		TWeakObjectPtr<UKartFrictionComponent> WeakThis = this;
		auto TimerDelegate = FTimerDelegate::CreateLambda([WeakThis]() {
			if (WeakThis.IsValid())
			{
				UKartFrictionComponent* StrongThis = WeakThis.Get();
				StrongThis->bForceDrfit = false;
			}
		});
		GetWorld()->GetTimerManager().SetTimer(FrictionDelayTimer, TimerDelegate, MinimumFrictionDelay, false);
		bDrift = true;
		bForceDrfit = true;
		return ;
	}

	if (bDrift)
	{
		FVector RightVector = KartBody->GetRightVector();
		FVector Point = Kart->GetLR_Wheel()->GetComponentLocation() + Kart->GetRR_Wheel()->GetComponentLocation();
		Point /= 2.0f;
		FVector LinearVelocity = KartBody->GetPhysicsLinearVelocityAtPoint(Point);
		float Velocity = FVector::DotProduct(RightVector, LinearVelocity);
		float TotalVelocity = LinearVelocity.Size();

		constexpr float SlipAngleThreshold = 30.0f; // degree
		constexpr float LateralRatioThreshold = 0.7f; // 20% 이상 미끄러지는 경우

		FVector WheelForwardVector = Kart->GetLF_Wheel()->GetForwardVector();
		float WheelForwardVelocity = FVector::DotProduct(WheelForwardVector, LinearVelocity);
		FVector WheelRightVector = Kart->GetLF_Wheel()->GetRightVector();
		float WheelRightVelocity = FVector::DotProduct(WheelRightVector, LinearVelocity);

		FRotator WheelRot = Kart->GetLF_Wheel()->GetRelativeRotation();
		float SteeringAngle = FMath::DegreesToRadians(WheelRot.Yaw);

		float SlipAngle = FMath::Atan2(WheelRightVelocity, WheelForwardVelocity) - SteeringAngle;
		// float SlipAngle = FMath::Atan2(ForwardVelocity, RightVelocity) - SteeringAngle;
		float LateralRatio = TotalVelocity > KINDA_SMALL_NUMBER ? FMath::Abs(Velocity) / TotalVelocity : 0.0f;

		bDrift = (FMath::Abs(FMath::RadiansToDegrees(SlipAngle)) > SlipAngleThreshold)
				|| (LateralRatio > LateralRatioThreshold)
				|| (bDriftInput && bSteering)
				|| bForceDrfit;
		bDrift = bDrift && bFlag;
	}
}

void UKartFrictionComponent::BroadCastDriftEnd()
{
	if (bPrevDrift && !bDrift)
	{
		DriftGaugeOffset = DriftGauge - DriftGaugeStart;
		// 드리프트 게이지가 일정 이상이면 인스턴트 부스트 드리프트 종료 이벤트를 브로드캐스트
		if (DriftGaugeOffset > DriftGaugeThreshold)
		{
			OnInstantBoost.Broadcast();
		}
		if (DriftGauge >= DriftGaugeMax)
		{
			DriftGauge = 0.0f;
			OnBoosterMade.Broadcast();
			OnBoosterGaugeUpdated.Broadcast(DriftGauge, DriftGaugeMax);
		}
		OnDriftEnded.Broadcast();
		bDriftStartOnce = false;
	}
	else if (bPrevDrift && bDrift && !bDriftStartOnce)
	{
		bDriftStartOnce = true;
		DriftGaugeStart = DriftGauge;
		OnDriftStarted.Broadcast();
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
		WheelLR = Kart->GetLR_Wheel();
		WheelRR = Kart->GetRR_Wheel();

		Kart->GetItemInteractionComponent()->InteractionDelegate.AddDynamic(this, &UKartFrictionComponent::OnItemInteraction);
	}
}

void UKartFrictionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Type::Destroyed)
	{
		GetWorld()->GetTimerManager().ClearTimer(FrictionDelayTimer);
	}
	
	Super::EndPlay(EndPlayReason);
}

void UKartFrictionComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Drift, ETriggerEvent::Completed, this, &UKartFrictionComponent::OnDriftInputDetected);
	PlayerInputComponent->BindAction(IA_Drift, ETriggerEvent::Completed, this, &UKartFrictionComponent::OnBroadCastDriftKeyReleased);
	PlayerInputComponent->BindAction(IA_Drift, ETriggerEvent::Started, this, &UKartFrictionComponent::OnBroadCastDriftKeyPressed);
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


void UKartFrictionComponent::OnItemInteraction()
{
	// 이 변수를 수정하면 자연스럽게 업데이트가 될 듯 : 드리프트가 눌린 상태에서 맞고 입력이 들어오면 드리프트가
	// 계속해서 유지되는 판정이 되려나?
	bDrift = false;
	bDriftInput = false;
	FFastLogger::LogConsole(TEXT("Drift End"));
}

void UKartFrictionComponent::OnBroadCastDriftKeyReleased(const FInputActionValue& InputActionValue)
{
	OnDriftKeyPressed.Broadcast(false);
}

void UKartFrictionComponent::OnBroadCastDriftKeyPressed(const FInputActionValue& InputActionValue)
{
	OnDriftKeyPressed.Broadcast(true);
}

void UKartFrictionComponent::UpdateBoosterGauge()
{
	if (!bDrift) return;

	// 드리프트 게이지를 증가 시킨다.
	// 근데 Lateral Force가 얼마나 작용하는지에 따라 게이지가 증가하는 속도가 달라진다.
	FVector RightVector = KartBody->GetRightVector();
	FVector BackwardPosition = (WheelLR->GetComponentLocation() + WheelRR->GetComponentLocation()) / 2.0f;

	FVector VelocityAtBackward = KartBody->GetPhysicsLinearVelocityAtPoint(BackwardPosition);
	
	float LateralForce = FMath::Abs(FVector::DotProduct(RightVector, VelocityAtBackward));
	// Lateral Force를 정규화 해야 하는데
	float NormalizedLateralForce = LateralForce / Kart->GetMaxSpeed();
	NormalizedLateralForce = FMath::Clamp(NormalizedLateralForce, 0.2f, NormalizedLateralForce);
	DriftGauge += DriftGaugeSpeed * NormalizedLateralForce * GetWorld()->GetDeltaSeconds();

	OnBoosterGaugeUpdated.Broadcast(DriftGauge, DriftGaugeMax);
}