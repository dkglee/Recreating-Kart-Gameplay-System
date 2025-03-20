// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KartInfo.generated.h"

USTRUCT()
struct PROJECTR_API FKartInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform KartTransform;
	UPROPERTY()
	FVector Velocity;
	UPROPERTY()
	FVector TorqueInDegrees;
};
