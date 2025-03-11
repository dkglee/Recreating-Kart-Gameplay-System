// Fill out your copyright notice in the Description page of Project Settings.


#include "KartAccelerationComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Kart.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UKartAccelerationComponent::UKartAccelerationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_KARTMOVEMENT
	(TEXT("/Game/Kart/Input/InputAction/IA_KartMovement.IA_KartMovement"));
	if (IA_KARTMOVEMENT.Succeeded())
	{
		IA_Movement = IA_KARTMOVEMENT.Object;
	}
}

// Called when the game starts
void UKartAccelerationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartAccelerationComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		// 입력 바인딩 델리게이트 등록
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartAccelerationComponent::SetupInputBinding);
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());

		TArray<UKartSuspensionComponent*> FoundWheels;
		Kart->GetComponents<UKartSuspensionComponent>(FoundWheels);
		for (auto Wheel : FoundWheels)
		{
			Wheels.Add(Wheel);
		}
	}
}

void UKartAccelerationComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UKartAccelerationComponent::OnMovementInputDetected);
}

// Called every frame
void UKartAccelerationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ProcessAccleration(DeltaTime);
	ApplyForceToKart(DeltaTime);
	OnAccelerationDelegate.Broadcast(AccelerationInput);
}

void UKartAccelerationComponent::OnMovementInputDetected(const FInputActionValue& InputActionValue)
{
	float TargetAcceleration = InputActionValue.Get<float>();
	TargetAcceleration = FMath::Clamp(TargetAcceleration, -0.4f, 1.0f);
	AccelerationInput = FMath::FInterpTo(AccelerationInput, TargetAcceleration, GetWorld()->GetDeltaSeconds(), AccelerationRate);
}

void UKartAccelerationComponent::ProcessAccleration(float DeltaTime)
{
	Acceleration = MaxAcceleration * AccelerationInput;
	// 천천히 줄어듬
	AccelerationInput = FMath::FInterpTo(AccelerationInput, 0.0f, GetWorld()->GetDeltaSeconds(), DragCoefficient);
}

void UKartAccelerationComponent::ApplyForceToKart(float DeltaTime)
{
	FVector Force = KartBody->GetForwardVector() * Acceleration * KartBody->GetMass();
	
	for (int32 i = 0; i < Wheels.Num(); i++)
	{
		FVector Location = Wheels[i]->GetComponentLocation();
		KartBody->AddForceAtLocation(Force, Location);
	}
}
