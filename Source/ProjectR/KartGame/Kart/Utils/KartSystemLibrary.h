// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "KartSystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UKartSystemLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="Utilities")
	static float CalculateNormalizedSpeedWithBox(class UBoxComponent* Kart, float MaxSpeed);
	UFUNCTION(BlueprintPure, Category="Utilities")
	static float CalculateNormalizedSpeedWithVector(FVector ForwardVector, FVector LinearVelocity, float MaxSpeed);
	UFUNCTION(BlueprintPure, Category="Utilities")
	static float CalculateNormalizedSpeedWithKart(class AKart* Kart, float MaxSpeed);
	UFUNCTION(BlueprintPure, Category="Utilities")
	static float CalculateNormalizedRightSpeedWithKart(class UBoxComponent* KartBody, float MaxSpeed);
};
