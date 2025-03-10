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

		UE_LOG(LogTemp, Warning, TEXT("Kart Mass: %f"), KartBody->GetMass());
		
		TArray<UKartSuspensionComponent*> FoundWheels;
		Kart->GetComponents<UKartSuspensionComponent>(FoundWheels);
		for (auto Wheel : FoundWheels)
		{
			Wheels.Add(Wheel);
		}
		UE_LOG(LogTemp, Warning, TEXT("Wheels Num: %d"), Wheels.Num());
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
	// AlignToLandScape(DeltaTime);
}

void UKartAccelerationComponent::OnMovementInputDetected(const FInputActionValue& InputActionValue)
{
	float TargetAcceleration = InputActionValue.Get<float>();
	AccelerationInput = FMath::FInterpTo(AccelerationInput, TargetAcceleration, GetWorld()->GetDeltaSeconds(), AccelerationRate);
	UE_LOG(LogTemp, Warning, TEXT("AccelerationInput: %f"), AccelerationInput);
}

void UKartAccelerationComponent::ProcessAccleration(float DeltaTime)
{
	Acceleration = FMath::Lerp(0.0f, MaxAcceleration, AccelerationInput);
	if (FMath::IsNearlyZero(AccelerationInput))
	{
		Acceleration = 0.0f;
	}
	
	// 천천히 줄어듬
	AccelerationInput = FMath::FInterpTo(AccelerationInput, 0.0f, GetWorld()->GetDeltaSeconds(), DragCoefficient);
}

void UKartAccelerationComponent::ApplyForceToKart(float DeltaTime)
{
	FVector Force = KartBody->GetForwardVector() * KartBody->GetMass() * Acceleration;

	// KartBody->AddForceAtLocation(Force, KartBody->GetComponentLocation());
	
	for (int32 i = 0; i < Wheels.Num(); i++)
	{
		FVector Location = Wheels[i]->GetComponentLocation();
		KartBody->AddForceAtLocation(Force, Location);
		//
		// FVector Center = CenterOfMass * AccelerationInput;
		// KartBody->SetCenterOfMass(Center);
	}
}
