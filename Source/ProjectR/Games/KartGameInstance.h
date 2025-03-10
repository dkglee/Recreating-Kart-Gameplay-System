// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KartGameInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	int32 ItemWeight;	
};

UCLASS()
class PROJECTR_API UKartGameInstance : public UGameInstance
{
	GENERATED_BODY()
};
