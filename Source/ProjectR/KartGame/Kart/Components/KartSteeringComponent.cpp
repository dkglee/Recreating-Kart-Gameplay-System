// Fill out your copyright notice in the Description page of Project Settings.
#include "KartSteeringComponent.h"

#include "EnhancedInputComponent.h"
#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartSuspensionComponent.h"
#include "KartSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "Math/BigInt.h"

// Sets default values for this component's properties
UKartSteeringComponent::UKartSteeringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	// ...
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_STEERING
	(TEXT("/Game/Kart/Input/InputAction/IA_KartSteering.IA_KartSteering"));
	if (IA_STEERING.Succeeded())
	{
		IA_Steering = IA_STEERING.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> C_STEERINGCURVE
	(TEXT("/Game/Kart/Curves/SteeringCurve.SteeringCurve"));
	if (C_STEERINGCURVE.Succeeded())
	{
		SteeringCurve = C_STEERINGCURVE.Object;
	}
}


// Called when the game starts
void UKartSteeringComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartSteeringComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartSteeringComponent::SetupInputBinding);
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
	}
}

void UKartSteeringComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Steering, ETriggerEvent::Triggered, this, &UKartSteeringComponent::OnSteeringInputDetected);
	PlayerInputComponent->BindAction(IA_Steering, ETriggerEvent::Completed, this, &UKartSteeringComponent::OnSteeringInputDetected);
}

// Called every frame
void UKartSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UKartSteeringComponent::ProcessSteering()
{
	ApplySteeringToKart_Implementation(TargetSteering);
}

void UKartSteeringComponent::ProcessTorque()
{
	ApplyTorqueToKartV2_Implementation(SteeringIntensity);
}

void UKartSteeringComponent::OnSteeringInputDetected(const FInputActionValue& InputActionValue)
{
	TargetSteering = InputActionValue.Get<float>();
}

// 해당 함수는 앞바퀴를 회전하는 용도로 사용될 거임
void UKartSteeringComponent::ApplySteeringToKart_Implementation(float InTargetSteering)
{
	// 카트 바디의 앞바퀴들을 가져옴
	UKartSuspensionComponent* LF_Wheel = Kart->GetLF_Wheel();
	UKartSuspensionComponent* RF_Wheel = Kart->GetRF_Wheel();

	TArray<UKartSuspensionComponent*> Wheels = {LF_Wheel, RF_Wheel};

	float TempSteerRate = FMath::Abs(InTargetSteering) ? SteerRate * 20.0f : SteerRate * 3.0f;

	SteeringIntensity = FMath::FInterpTo(SteeringIntensity, InTargetSteering, GetWorld()->GetDeltaSeconds(), TempSteerRate);
	
	// Rotate the wheels
	for (UKartSuspensionComponent* Wheel : Wheels)
	{
		float Alpha = (SteeringIntensity + 1.0f) / 2.0f;
		FRotator NewRotation = FMath::Lerp(FRotator{0, -MaxRotation, 0}, FRotator{0, MaxRotation, 0}, Alpha);
		Wheel->SetRelativeRotation(NewRotation);
	}
}

// 해당 함수는 실질적으로 Kart Body에 Torque를 가할 때 사용될 거임
void UKartSteeringComponent::ApplyTorqueToKartV2_Implementation(float InSteering)
{
	float InNormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());
	float SteeringPower = SteeringCurve->GetFloatValue(InNormalizedSpeed);

	FVector ForwardVector = KartBody->GetForwardVector();
	FVector LinearVelocity = KartBody->GetPhysicsLinearVelocity();
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);
	float KartSign = FMath::Sign(KartSpeed);
	
	float Force = InSteering * SteeringPower * TurnScaling * KartSign;

	// Torque Vector 생성
	FVector KartUpVector = KartBody->GetUpVector();
	FVector Torque = KartUpVector * Force;

	// KartBody에 Torque 적용
	KartBody->AddTorqueInDegrees(Torque, NAME_None, true);
}
