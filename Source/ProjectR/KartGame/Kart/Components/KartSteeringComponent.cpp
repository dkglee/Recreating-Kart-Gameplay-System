// Fill out your copyright notice in the Description page of Project Settings.
#include "KartSteeringComponent.h"

#include "EnhancedInputComponent.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "Components/BoxComponent.h"
#include "Math/BigInt.h"

// Sets default values for this component's properties
UKartSteeringComponent::UKartSteeringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// ...
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_STEERING
	(TEXT("/Game/Kart/Input/InputAction/IA_KartSteering.IA_KartSteering"));
	if (IA_STEERING.Succeeded())
	{
		IA_Steering = IA_STEERING.Object;
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
		UKartAccelerationComponent* Comp = Cast<UKartAccelerationComponent>(Kart->GetComponentByClass(UKartAccelerationComponent::StaticClass()));
		if (Comp)
		{
			Comp->OnAccelerationDelegate.AddDynamic(this, &UKartSteeringComponent::OnAccelerationInputDetected);
		}
	}
}

void UKartSteeringComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Steering, ETriggerEvent::Triggered, this, &UKartSteeringComponent::OnSteeringInputDetected);
}

void UKartSteeringComponent::OnAccelerationInputDetected(float InAccelerationIntensity)
{
	AccelerationIntensity = InAccelerationIntensity;
}

// Called every frame
void UKartSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ApplyTorqueToKart();
	ProcessSteering();
}

void UKartSteeringComponent::ApplyTorqueToKart()
{
	FVector ForwardVector = KartBody->GetForwardVector();
	FVector Velocity = KartBody->GetComponentVelocity();
	ForwardVector.Normalize();
	Velocity.Normalize();
	float ForwardIntensity = FVector::DotProduct(ForwardVector, Velocity);
	
	float Torque = SteeringIntenstiy * SteerPower * FMath::Sign(ForwardIntensity);

	// 종방향 속도가 없을 경우 바디에 토크를 가하지 않음
	if (FMath::Abs(AccelerationIntensity) > 0.05f)
	{
		KartBody->AddTorqueInRadians({0, 0, Torque});
	}
}

void UKartSteeringComponent::ProcessSteering()
{
	SteeringIntenstiy = FMath::FInterpTo(SteeringIntenstiy, 0.0f, GetWorld()->GetDeltaSeconds(), DampingCoefficient);
}

void UKartSteeringComponent::OnSteeringInputDetected(const FInputActionValue& InputActionValue)
{
	float TargetSteer = InputActionValue.Get<float>();
	SteeringIntenstiy = FMath::FInterpTo(SteeringIntenstiy, TargetSteer, GetWorld()->GetDeltaSeconds(), SteerRate);
}
