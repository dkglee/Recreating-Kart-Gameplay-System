// Fill out your copyright notice in the Description page of Project Settings.


#include "KartCameraComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartBoosterComponent.h"
#include "KartSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values for this component's properties
UKartCameraComponent::UKartCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UKartCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartCameraComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartCamera = Kart->GetCameraComponent();
		KartSpringArm = Kart->GetSpringArmComponent();

		KartSpringArm->CameraRotationLagSpeed = 4.0f;
		DefaultSpringArmLength = KartSpringArm->TargetArmLength;
		TargetSpringArmLength = DefaultSpringArmLength;
		
		KartBoosterComponent = Kart->GetBoosterComponent();

		if (KartBoosterComponent)
		{
			KartBoosterComponent->OnBoosterActivated.AddDynamic(this, &UKartCameraComponent::OnBoosterActivated);
			KartBoosterComponent->OnBoosterDeactivated.AddDynamic(this, &UKartCameraComponent::SetDefaultSpringArmLength);
			KartBoosterComponent->OnInstantBoosterActivated.AddDynamic(this, &UKartCameraComponent::OnInstantBoosterActivated);
			KartBoosterComponent->OnInstantBoosterDeactivated.AddDynamic(this, &UKartCameraComponent::SetDefaultSpringArmLength);
		}
	}
}

void UKartCameraComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimer);
	}
	
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UKartCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Kart->IsLocallyControlled()) return ;

	// 카메라 거리를 조절
	KartSpringArm->TargetArmLength = FMath::FInterpTo(KartSpringArm->TargetArmLength, TargetSpringArmLength, DeltaTime, 1.0f);
}

void UKartCameraComponent::OnBoosterActivated(float BoosterTime)
{
	TargetSpringArmLength = BoosterTargetSpringArmLegnth;
	
	GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimer);
	GetWorld()->GetTimerManager().SetTimer(CameraLerpTimer, this, &UKartCameraComponent::SetDefaultSpringArmLength, BoosterTime, false);
}

void UKartCameraComponent::OnInstantBoosterActivated(float BoosterTime)
{
	TargetSpringArmLength = InstantTargetSpringArmLength;
	
	GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimer);
	GetWorld()->GetTimerManager().SetTimer(CameraLerpTimer, this, &UKartCameraComponent::SetDefaultSpringArmLength, BoosterTime, false);
}

void UKartCameraComponent::SetDefaultSpringArmLength()
{
	GetWorld()->GetTimerManager().ClearTimer(CameraLerpTimer);
	
	TargetSpringArmLength = DefaultSpringArmLength;
}
