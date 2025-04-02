// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkInterpolationStrategy.h"
#include "DeadReckoningStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UDeadReckoningStrategy : public UNetworkInterpolationStrategy
{
	GENERATED_BODY()

public:
	virtual void Initialize(class AKart* InKart) override;
	virtual void Update(float DeltaTime) override;
	virtual void UpdateRemoteState(const FKartInfo& NewKartInfo) override;

private:
	UPROPERTY()
	class AKart* Kart = nullptr;
	UPROPERTY()
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY()
	FKartInfo LastUpdatedKartInfo;

	UPROPERTY()
	FVector PredictedPosition;
	UPROPERTY()
	FRotator PredictedRotation;
};
