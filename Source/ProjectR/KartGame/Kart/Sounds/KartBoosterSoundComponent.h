// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "KartBoosterSoundComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UKartBoosterSoundComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartBoosterSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	UFUNCTION()
	void OnBoosterActivated(float BoosterTime);
	UFUNCTION()
	void OnInstantBoosterActivated(float BoosterTime);
	UFUNCTION()
	void OnBoostersDeactivated();

	UPROPERTY()
	class AKart* Kart = nullptr;
	
	UPROPERTY()
	class USoundCue* BoosterSound;

	UPROPERTY()
	class USoundCue* InstantBoosterSound;
};
