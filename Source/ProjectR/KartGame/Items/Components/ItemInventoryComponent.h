// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectR/KartGame/Items/DataTable/ItemTable.h"
#include "ItemInventoryComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UItemInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemInventoryComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void GetItem(FItemTable itemData);
	
	void UseItem();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FItemTable> Inventory;

	UPROPERTY()
	bool bInventoryIsFull = false;
};
