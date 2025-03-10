// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSteeringComponentLegacy.h"

#include "EnhancedInputComponent.h"
#include "Kart.h"


// Sets default values for this component's properties
UKartSteeringComponentLegacy::UKartSteeringComponentLegacy()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_STEERING
	(TEXT("/Game/Kart/Input/InputAction/IA_KartSteering.IA_KartSteering"));
	if (IA_STEERING.Succeeded())
	{
		IA_Steering = IA_STEERING.Object;
	}
}


// Called when the game starts
void UKartSteeringComponentLegacy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UKartSteeringComponentLegacy::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 조향 처리
	ProcessSteering();
	OnSteeringDelegate.Broadcast(SteeringAngle);
	SteeringInput = 0.0f;
}


void UKartSteeringComponentLegacy::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartSteeringComponentLegacy::SetupInputBinding);
	}
}

void UKartSteeringComponentLegacy::SetupInputBinding(class UEnhancedInputComponent* PlayerInput)
{
	PlayerInput->BindAction(IA_Steering, ETriggerEvent::Triggered, this, &UKartSteeringComponentLegacy::OnSteeringInputDetected);
}

void UKartSteeringComponentLegacy::OnSteeringInputDetected(const FInputActionValue& InputActionValue)
{
	SteeringInput = InputActionValue.Get<float>();
}

void UKartSteeringComponentLegacy::ProcessSteering()
{
	float TargetSteer = FMath::Abs(SteeringInput) > 0.0f ? 1.0f : 0.0f;
	TargetSteer = FMath::FInterpTo(Steer, TargetSteer, GetWorld()->GetDeltaSeconds(), SteeringRate);
	Steer = TargetSteer;
	
	float MaxSteeringAngleRad = FMath::DegreesToRadians(MaxSteeringAngle);
	SteeringAngle = MaxSteeringAngleRad * Steer * -SteeringInput;
}
