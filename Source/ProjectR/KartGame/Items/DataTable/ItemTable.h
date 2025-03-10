// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FItemTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	FItemTable() : ItemName(""), ItemWeight(0) {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	FName ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Item)
	int32 ItemWeight;	
};
