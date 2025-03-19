// Fill out your copyright notice in the Description page of Project Settings.


#include "KartDriftSoundComponent.h"

#include "Kart.h"
#include "Components/BoxComponent.h"


// Sets default values for this component's properties
UKartDriftSoundComponent::UKartDriftSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(false);
	// ...

	static ConstructorHelpers::FObjectFinder<USoundBase> MSS_DRIFT
	(TEXT("/Game/Kart/Sounds/MSS_Drift.MSS_Drift"));
	if (MSS_DRIFT.Succeeded())
	{
		DriftSoundSource = MSS_DRIFT.Object;
		SetSound(DriftSoundSource);
	}

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> SA_DRIFT
	(TEXT("/Game/Kart/Sounds/SA_Engine.SA_Engine"));
	if (SA_DRIFT.Succeeded())
	{
		DriftSoundAttenuation = SA_DRIFT.Object;
		SetAttenuationSettings(DriftSoundAttenuation);
	}
}


// Called when the game starts
void UKartDriftSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Play();
	// SetVolumeMultiplier(MaxDriftVolume);
}

void UKartDriftSoundComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
		KartNetworkSyncComponent = Kart->GetNetworkSyncComponent();
	}
}

void UKartDriftSoundComponent::PlayDriftSound(bool bDrift)
{
	DrawDebugString(GetWorld(), KartBody->GetComponentLocation(), *FString::Printf(TEXT("Bool: %d"), bDrift), nullptr, FColor::Red, 0.0f);
	SetBoolParameter(TEXT("bDrift"), bDrift);
}
