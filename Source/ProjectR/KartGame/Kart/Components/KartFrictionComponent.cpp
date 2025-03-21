// Fill out your copyright notice in the Description page of Project Settings.


#include "KartFrictionComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FastLogger.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kart.h"
#include "KartSteeringComponent.h"
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
}


// Called when the game starts
void UKartFrictionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKartFrictionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKartFrictionComponent::ProcessFriction()
{
	// 입력과 현재 상태에 따라 드리프트를 적용할 지 말지 정함
	DetermineDriftState();
	// 드리프트 상태에 따라 마찰력을 적용
	SetAngularDampling();
	ApplyFrictionToKart_Implementation(bDrift);
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
		KartBody->SetAngularDamping(HardDrfitAngularDamping);
		DrawDebugString(GetWorld(), KartBody->GetComponentLocation(), TEXT("Hard Drift"), nullptr, FColor::Red, 0.0f);
		float Threshold = 0.5f;
		if (NormalizedSpeed < Threshold)
		{
			KartBody->SetAngularDamping(DefaultAngularDamping * 0.9f);
		}
	}
	else if (!bDriftInput && bSteering && bDrift)
	{
		// 드리프트 버튼이 안눌리고 방향키 입력만으로 드리프트를 유지하고 있는 경우
		KartBody->SetAngularDamping(NormalAngularDamping);
		DrawDebugString(GetWorld(), KartBody->GetComponentLocation(), TEXT("Normal Drift"), nullptr, FColor::Red, 0.0f);
		float Threshold = 0.5f;
		if (NormalizedSpeed < Threshold)
		{
			KartBody->SetAngularDamping(DefaultAngularDamping * 0.9f);
		}
	}
	else if (!bDriftInput && !bSteering)
	{
		// 드리프트 버튼이 안눌리고 방향키 입력도 없는 경우
		KartBody->SetAngularDamping(DefaultAngularDamping);
		DrawDebugString(GetWorld(), KartBody->GetComponentLocation(), TEXT("Default Drift : No Drift No Steer"), nullptr, FColor::Red, 0.0f);
	}
	else
	{
		// 나머지 경우
		DrawDebugString(GetWorld(), KartBody->GetComponentLocation(), TEXT("Default Drift"), nullptr, FColor::Red, 0.0f);
		KartBody->SetAngularDamping(DefaultAngularDamping);
	}

}

void UKartFrictionComponent::DetermineDriftState()
{
	// 속도를 구함
	float NormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());
	bool bSteering = !FMath::IsNearlyZero(FMath::Abs(Kart->GetSteeringComponent()->GetTargetSteering()));

	// 속도가 일정 이상이다. 이전에 드리프트 상태가 아니었다면 드리프트 상태로 변경
	constexpr float Threshold = 0.1f; 
	bool bFlag = NormalizedSpeed > Threshold;
	if (bFlag && !bDrift && bDriftInput && bSteering)
	{
		// 드리프트 상태로 변경 후 리턴
		bDrift = true;
		return ;
	}

	if (bDrift)
	{
		float AngularVelocity = KartBody->GetPhysicsAngularVelocityInDegrees().Length();

		FVector RightVector = KartBody->GetRightVector();
		FVector LinearVelocity = KartBody->GetPhysicsLinearVelocity();
		float Velocity = FVector::DotProduct(RightVector, LinearVelocity);

		// 오른쪽으로 가는 속도가 있다면 드리프트 상태로 유지
		constexpr float AngularDriftThreshold = 5.0f;
		constexpr float DriftThreshold = 400.0f;
		
		// bDrift = bFlag && FMath::Abs(AngularVelocity) > AngularDriftThreshold;
		// bDrift &= bFlag && FMath::Abs(Velocity) > DriftThreshold;
		bDrift = FMath::Abs(AngularVelocity) > AngularDriftThreshold;
		bDrift &= FMath::Abs(Velocity) > DriftThreshold;
		bDrift = bDrift || (bDriftInput && bSteering);

		//
		// FVector ForwardVector = KartBody->GetForwardVector();
		// float  = FVector::DotProduct(ForwardVector, LinearVelocity);
		// float 
	}
	DrawDebugString(GetWorld(), KartBody->GetComponentLocation() + FVector(0, 0, -50), *FString::Printf(TEXT("Steer: %f"), Kart->GetSteeringComponent()->GetSteeringIntensity()), nullptr, FColor::Red, 0.0f);
	DrawDebugString(GetWorld(), KartBody->GetComponentLocation() + FVector(0, 0, -25), *FString::Printf(TEXT("Angular: %f"), KartBody->GetPhysicsAngularVelocityInDegrees().Length()), nullptr, FColor::Red, 0.0f);
	
	DrawDebugString(GetWorld(), KartBody->GetComponentLocation() + FVector(0, 0, 50), *FString::Printf(TEXT("bDrift : %s"), bDrift ? TEXT("true") : TEXT("false")), nullptr, FColor::Red, 0.0f);
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
	float TargetInFrictionGrip = bInDrift ? 1.0f : FrictionGrip;

	// 보간 속도 계수 (너무 높으면 여전히 급격하고, 너무 낮으면 느리게 반응)
	float InterpSpeed = 3.0f; // 적절히 조절

	// Lerp 또는 InterpTo를 활용한 부드러운 전환
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	if (bDrift)
	{
		InFrictionData = TargetFrictionData;
		InFrictionGrip = TargetInFrictionGrip;
	}
	else
	{
		InFrictionData = FMath::FInterpTo(InFrictionData, TargetFrictionData, DeltaTime, InterpSpeed);
		InFrictionGrip = FMath::FInterpTo(InFrictionGrip, TargetInFrictionGrip, DeltaTime, InterpSpeed);
	}
	
	FVector FrictionForce = RightVector * Velocity * -1.5f * InFrictionData * InFrictionGrip;

	KartBody->AddForce(FrictionForce, NAME_None, true);
}
