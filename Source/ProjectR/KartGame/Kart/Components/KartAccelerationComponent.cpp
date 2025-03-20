// Fill out your copyright notice in the Description page of Project Settings.


#include "KartAccelerationComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Kart.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UKartAccelerationComponent::UKartAccelerationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

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

		Wheels.Empty();
		Wheels.Add(Kart->GetLR_Wheel());
		Wheels.Add(Kart->GetRR_Wheel());
	}
}

void UKartAccelerationComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UKartAccelerationComponent::OnMovementInputDetected);
	PlayerInputComponent->BindAction(IA_Movement, ETriggerEvent::Completed, this, &UKartAccelerationComponent::OnMovementInputDetected);
}

void UKartAccelerationComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void UKartAccelerationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UKartAccelerationComponent::OnMovementInputDetected(const FInputActionValue& InputActionValue)
{
	TargetAcceleration = InputActionValue.Get<float>();
}

void UKartAccelerationComponent::ProcessAcceleration(bool bGameStart)
{
	if (bGameStart)
	{
		ApplyForceToKart_Implementation();
	}
}

void UKartAccelerationComponent::ApplyForceToKart_Implementation()
{
	TargetAcceleration = FMath::Clamp(TargetAcceleration, -0.7f, 1.0f);
	AccelerationInput = FMath::FInterpTo(AccelerationInput, TargetAcceleration, GetWorld()->GetDeltaSeconds(), AccelerationRate);
	Acceleration = MaxAcceleration * AccelerationInput;
	
	// 천천히 줄어듬
	FVector Forward = KartBody->GetForwardVector();
	FVector Velocity = KartBody->GetPhysicsLinearVelocity();
	float ForwardSpeed = FVector::DotProduct(Forward, Velocity); // cm/s

	// MaxSpeed 제한 (MaxSpeed는 cm/s 기준이어야 함) // 부스터 사용 X
	if (ForwardSpeed >= Kart->GetMaxSpeed() && Acceleration > 0.0f)
	{
		// 이미 MaxSpeed 이상인데 더 가속하는 경우, 무시
		return;
	}
	
	FVector Force = KartBody->GetForwardVector() * Acceleration * KartBody->GetMass();
	
	for (int32 i = 0; i < Wheels.Num(); i++)
	{
		FVector Location = Wheels[i]->GetComponentLocation();
		KartBody->AddForceAtLocation(Force, Location);
	}
}

void UKartAccelerationComponent::ClearAcceleration()
{
	AccelerationInput = 0.f;
	Acceleration = 0;
	KartBody->SetPhysicsLinearVelocity(FVector::ZeroVector);
}
