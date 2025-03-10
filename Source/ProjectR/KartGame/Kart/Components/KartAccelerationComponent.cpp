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
	AlignToLandScape(DeltaTime);
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
	FVector Force = KartBody->GetForwardVector() * KartBody->GetMass() * Acceleration * 0.4;

	KartBody->AddForceAtLocation(Force, KartBody->GetComponentLocation());
	
	// for (int32 i = 0; i < Wheels.Num(); i++)
	// {
	// 	FVector Location = Wheels[i]->GetComponentLocation();
	// 	KartBody->AddForceAtLocation(Force, Location);
	// 	
	// 	FVector Center = CenterOfMass * AccelerationInput;
	// 	KartBody->SetCenterOfMass(Center);
	// }
}

void UKartAccelerationComponent::AlignToLandScape(float DeltaTime)
{
	FVector LandScapeNormalAvg = FVector::ZeroVector;
	FVector LandScapePointAvg = FVector::ZeroVector; // 추가: 지면 평균 위치
	int32 Num = 0;
	
	for (int32 i = 0; i < Wheels.Num(); i++)
	{
		FVector TempNormal, TempLocation;
		if (Wheels[i]->GetLandScapeNormal(TempNormal, TempLocation)) // GetLandScapeNormal 함수에서 지면 위치도 반환하도록 수정 필요!
		{
			LandScapeNormalAvg += TempNormal;
			LandScapePointAvg += TempLocation; // 지면의 위치 합산
			Num++;
		}
	}
	
	if (Num <= 2)
		return;
	
	LandScapeNormalAvg /= Num;
	LandScapeNormalAvg.Normalize();
	LandScapePointAvg /= Num; // 평균 지면 위치 계산
	
	FVector TargetUp = LandScapeNormalAvg;
	FVector RightVector = KartBody->GetRightVector();
	FRotator TargetRotation = UKismetMathLibrary::MakeRotFromZX(TargetUp, RightVector);
	
	// KartBody->SetWorldRotation(TargetRotation);
	
	// 목표 위치: 평균 지면 위치에서 50.0f 위
	FVector TargetLocation = LandScapePointAvg + (LandScapeNormalAvg * 20.0f);
	KartBody->SetWorldLocation(TargetLocation);
}

