// Fill out your copyright notice in the Description page of Project Settings.


#include "KartBoosterSoundComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartBoosterComponent.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Sound/SoundCue.h"

UKartBoosterSoundComponent::UKartBoosterSoundComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(false);
	
	static ConstructorHelpers::FObjectFinder<USoundCue> SC_BOOSTER
	(TEXT("/Game/Kart/Sounds/C_BoosterSound.C_BoosterSound"));
	if (SC_BOOSTER.Succeeded())
	{
		BoosterSound = SC_BOOSTER.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> SC_INSTANT_BOOSTER
	(TEXT("/Game/Kart/Sounds/C_InstantBoosterSound.C_InstantBoosterSound"));
	if (SC_INSTANT_BOOSTER.Succeeded())
	{
		InstantBoosterSound = SC_INSTANT_BOOSTER.Object;
	}
}

void UKartBoosterSoundComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UKartBoosterSoundComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->GetBoosterComponent()->OnBoosterActivated.AddDynamic(this, &UKartBoosterSoundComponent::OnBoosterActivated);
		Kart->GetBoosterComponent()->OnInstantBoosterActivated.AddDynamic(this, &UKartBoosterSoundComponent::OnInstantBoosterActivated);
		Kart->GetBoosterComponent()->OnBoosterDeactivated.AddDynamic(this, &UKartBoosterSoundComponent::OnBoostersDeactivated);
		Kart->GetBoosterComponent()->OnBoosterDeactivated.AddDynamic(this, &UKartBoosterSoundComponent::OnBoostersDeactivated);
	}
}

void UKartBoosterSoundComponent::OnBoosterActivated(float BoosterTime)
{
	SetSound(BoosterSound);
	Play();
}

void UKartBoosterSoundComponent::OnInstantBoosterActivated(float BoosterTime)
{
	SetSound(InstantBoosterSound);
	Play();
}

void UKartBoosterSoundComponent::OnBoostersDeactivated()
{
	// 모든 부스터 끄기
	Deactivate();
}
