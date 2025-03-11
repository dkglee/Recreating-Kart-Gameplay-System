// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemTable.generated.h"

/**
 * 
 */
UENUM()
enum EItemName
{
	None,
	Booster,
	Missile,
	WaterBomb,
	Shield,
};
USTRUCT(BlueprintType)
struct PROJECTR_API FItemTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemTable() : ItemName(""), ItemWeight(0), ItemType(false) {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	int32 ItemWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	bool ItemType; // true = 조준형 , false = 즉발형
};
