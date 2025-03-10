// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "Kart.h"


// Sets default values for this component's properties
UItemInventoryComponent::UItemInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	ConstructorHelpers::FObjectFinder<UInputAction> ia_useitem(TEXT("'/Game/Kart/Input/InputAction/IA_UseItem.IA_UseItem'"));
	if (ia_useitem.Succeeded())
	{
		IA_UseItem = ia_useitem.Object;
	}
}


// Called when the game starts
void UItemInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UItemInventoryComponent::SetupInputBinding);
	}
}

void UItemInventoryComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_UseItem, ETriggerEvent::Started, this, &UItemInventoryComponent::UseItem);
}


// Called every frame
void UItemInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemInventoryComponent::GetItem(FItemTable itemData)
{
	if (bInventoryIsFull)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Full!"));
		return;
	}

	Inventory.Add(itemData);

	if (Inventory.Num() == 2)
	{
		bInventoryIsFull = true;
	}
}

void UItemInventoryComponent::UseItem()
{
	if (Inventory.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Empty!"));
		return;
	}
	
	FItemTable usingItem = Inventory[0];
	UE_LOG(LogTemp, Warning, TEXT("Using Item : %s"), *usingItem.ItemName.ToString());

	Inventory.RemoveAt(0);
	// // 두번째 인벤토리에 아이템이 있으면 당겨오기
	// if (Inventory.Num() == 1)
	// {
	// 	Inventory[0] = Inventory[1];
	// 	Inventory.RemoveAt(1);
	// }
}

