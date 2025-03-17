// Fill out your copyright notice in the Description page of Project Settings.


#include "KartFrictionComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FastLogger.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kart.h"


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
	ApplyFrictionToKart_Implementation(bDrift);
}

void UKartFrictionComponent::OnDriftInputDetected(const FInputActionValue& InputActionValue)
{
	bDrift = InputActionValue.Get<bool>();
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
	if (!bInDrift)
	{
		KartBody->SetAngularDamping(3.5f);
	}
	else
	{
		KartBody->SetAngularDamping(0.9f);
	}
	
	// Base 드리프트 입력하지 않을 경우 마찰력을 최대로 함
	FVector RightVector = KartBody->GetRightVector();
	FVector LinearVelocity = KartBody->GetPhysicsLinearVelocity();
	float Velocity = FVector::DotProduct(RightVector, LinearVelocity);

	FVector FrictionForce = RightVector * Velocity * -1.5f * 1.0f * 10.0f;

	KartBody->AddForce(FrictionForce, NAME_None, true);
}
