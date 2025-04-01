// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBombSoundComponent.h"


// Sets default values for this component's properties
UWaterBombSoundComponent::UWaterBombSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(false);

	static ConstructorHelpers::FObjectFinder<USoundBase> waterBombExplosionSound (TEXT("'/Game/Items/Sounds/WaterBombExplosion.WaterBombExplosion'"));
	if (waterBombExplosionSound.Succeeded())
	{
		WaterBombExplosionSoundSource = waterBombExplosionSound.Object;
	}
	
}


// Called when the game starts
void UWaterBombSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWaterBombSoundComponent::PlayWaterBombExplosionSound()
{
	SetSound(WaterBombExplosionSoundSource);
	Play();
}

