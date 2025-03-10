// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryComponent.h"


// Sets default values for this component's properties
UItemInventoryComponent::UItemInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UItemInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UItemInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemInventoryComponent::GetItem(FItemTable itemData)
{
	if (bInventoryIsFull) return;

	Inventory.Add(itemData);

	if (Inventory.Num() == 2)
	{
		bInventoryIsFull = true;
	}
}

void UItemInventoryComponent::UseItem()
{
	FItemTable usingItem = Inventory[0];
	UE_LOG(LogTemp, Warning, TEXT("Using Item : %s"), *usingItem.ItemName.ToString());

	Inventory.RemoveAt(0);
	Inventory[0] = Inventory[1];
	Inventory.RemoveAt(1);
}

