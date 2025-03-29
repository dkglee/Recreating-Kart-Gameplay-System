// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KartInfo.h"
#include "UObject/Object.h"
#include "NetworkInterpolationStrategy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UNetworkInterpolationStrategy : public UObject
{
	GENERATED_BODY()

public:
	virtual void Initialize(class AKart* InKart) {};
	virtual void Update(float DeltaTime) {};
	virtual void UpdateRemoteState(const FKartInfo& NewKartInfo) {};
};
