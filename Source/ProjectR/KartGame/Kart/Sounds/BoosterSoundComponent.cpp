// Fill out your copyright notice in the Description page of Project Settings.


#include "BoosterSoundComponent.h"

#include "Kart.h"
#include "KartBoosterComponent.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Sound/SoundCue.h"

UBoosterSoundComponent::UBoosterSoundComponent()
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

void UBoosterSoundComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UBoosterSoundComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart && Kart->IsLocallyControlled())
	{
		Kart->GetBoosterComponent()->OnBoosterActivated.AddDynamic(this, &UBoosterSoundComponent::OnBoosterActivated);
		Kart->GetBoosterComponent()->OnInstantBoosterActivated.AddDynamic(this, &UBoosterSoundComponent::OnInstantBoosterActivated);
		Kart->GetBoosterComponent()->OnBoosterDeactivated.AddDynamic(this, &UBoosterSoundComponent::OnBoostersDeactivated);
		Kart->GetBoosterComponent()->OnBoosterDeactivated.AddDynamic(this, &UBoosterSoundComponent::OnBoostersDeactivated);
	}
}

void UBoosterSoundComponent::OnBoosterActivated(float BoosterTime)
{
	SetSound(BoosterSound);
	Play();
}

void UBoosterSoundComponent::OnInstantBoosterActivated(float BoosterTime)
{
	SetSound(InstantBoosterSound);
	Play();
}

void UBoosterSoundComponent::OnBoostersDeactivated()
{
	// 모든 부스터 끄기
	Deactivate();
}
