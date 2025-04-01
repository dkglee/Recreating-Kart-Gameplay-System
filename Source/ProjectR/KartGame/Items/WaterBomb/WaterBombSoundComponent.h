// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "WaterBombSoundComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UWaterBombSoundComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UWaterBombSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void PlayWaterBombExplosionSound();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* WaterBombExplosionSoundSource;
};
