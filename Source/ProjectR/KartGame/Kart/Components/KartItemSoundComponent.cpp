// Fill out your copyright notice in the Description page of Project Settings.


#include "KartItemSoundComponent.h"


// Sets default values for this component's properties
UKartItemSoundComponent::UKartItemSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(false);

	static ConstructorHelpers::FObjectFinder<USoundBase> shieldSound (TEXT("'/Game/Items/Sounds/Shield.Shield'"));
	if (shieldSound.Succeeded())
	{
		ShieldSoundSource = shieldSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> waterBombThrowSound (TEXT("'/Game/Items/Sounds/WaterBombThrow.WaterBombThrow'"));
	if (waterBombThrowSound.Succeeded())
	{
		WaterBombThrowSoundSource = waterBombThrowSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> getItemSound (TEXT("'/Game/Items/Sounds/GetItemSound.GetItemSound'"));
	if (getItemSound.Succeeded())
	{
		GetItemSoundSource = getItemSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> missileMoveSound (TEXT("'/Game/Items/Sounds/MissileMove.MissileMove'"));
	if (missileMoveSound.Succeeded())
	{
		MissileMoveSoundSource = missileMoveSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> missileHitSound (TEXT("'/Game/Items/Sounds/MissileHit.MissileHit'"));
	if (missileHitSound.Succeeded())
	{
		MissileHitSoundSource = missileHitSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> missileLockOnSound (TEXT("'/Game/Items/Sounds/MissileLockOn.MissileLockOn'"));
	if (missileLockOnSound.Succeeded())
	{
		MissileLockOnSoundSource = missileLockOnSound.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> missileFindTargetSound (TEXT("'/Game/Items/Sounds/MissileFindTarget.MissileFindTarget'"));
	if (missileFindTargetSound.Succeeded())
	{
		MissileFindTargetSoundSource = missileFindTargetSound.Object;
	}


	
}


// Called when the game starts
void UKartItemSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UKartItemSoundComponent::PlayShieldSound()
{
	SetSound(ShieldSoundSource);
	Play();
}

void UKartItemSoundComponent::PlayWaterBombThrowSound()
{
	SetSound(WaterBombThrowSoundSource);
	Play();
}

void UKartItemSoundComponent::PlayGetItemSound()
{
	SetSound(GetItemSoundSource);
	Play();
}

void UKartItemSoundComponent::PlayMissileMoveSound()
{
	SetSound(MissileMoveSoundSource);
	Play();
}

void UKartItemSoundComponent::PlayMissileHitSound()
{
	SetSound(MissileHitSoundSource);
	Play();
}

void UKartItemSoundComponent::PlayMissileFindTargetSound()
{
	SetSound(MissileFindTargetSoundSource);
	Play();
}

void UKartItemSoundComponent::PlayMissileLockOnSound()
{
	SetSound(MissileLockOnSoundSource);
	Play();
}
