// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInventory.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UItemInventory : public UUserWidget
{
	GENERATED_BODY()

public:
	void GetItemUI(class UTexture2D* itemImage);

	void UseItemUI();

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* itemImage0;

	UPROPERTY(meta = (BindWidget))
	class UImage* itemImage1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 ItemCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxItemCount = 2;
};
