// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSteeringComponent.h"

#include "EnhancedInputComponent.h"
#include "Kart.h"


// Sets default values for this component's properties
UKartSteeringComponent::UKartSteeringComponent()
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
void UKartSteeringComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UKartSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 조향 처리
	ProcessSteering();
	OnSteeringDelegate.Broadcast(SteeringAngle);
}


void UKartSteeringComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartSteeringComponent::SetupInputBinding);
	}
}

void UKartSteeringComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInput)
{
	PlayerInput->BindAction(IA_Steering, ETriggerEvent::Triggered, this, &UKartSteeringComponent::OnSteeringInputDetected);
}

void UKartSteeringComponent::OnSteeringInputDetected(const FInputActionValue& InputActionValue)
{
	SteeringInput = InputActionValue.Get<float>();
}

void UKartSteeringComponent::ProcessSteering()
{
}
