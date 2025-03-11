// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "Kart.h"


UItemInventoryComponent::UItemInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	bWantsInitializeComponent = true;
	ConstructorHelpers::FObjectFinder<UInputAction> ia_useitem(TEXT("'/Game/Kart/Input/InputAction/IA_UseItem.IA_UseItem'"));
	if (ia_useitem.Succeeded())
	{
		IA_UseItem = ia_useitem.Object;
	}
}

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
	PlayerInputComponent->BindAction(IA_UseItem, ETriggerEvent::Ongoing, this, &UItemInventoryComponent::LockPlayer);
	PlayerInputComponent->BindAction(IA_UseItem, ETriggerEvent::Completed, this, &UItemInventoryComponent::UseItem);
}

void UItemInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemInventoryComponent::GetItem(const FItemTable itemData)
{ 
	if (bInventoryIsFull)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Full!"));
		return;
	}

	Inventory.Add(itemData);

	if (Inventory.Num() == MaxInventorySpace)
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
	//SpawnItem(usingItem);
	// 아이템 데이터(이름)을 바탕으로 
	Inventory.RemoveAt(0);
	bInventoryIsFull = false;
}

void UItemInventoryComponent::LockPlayer()
{
	if (!Inventory[0].ItemType) return;
	FItemTable usingItem = Inventory[0];

	// 사용자 정면 기준으로 lay를 쏜다
	// lay안에 상대가 있으면 그 상대를 instance에 저장한다
	// 해당 instance를 조준형 아이템에 전달한다.
	FVector start = Kart->GetActorLocation();
	FVector end = Kart->GetActorForwardVector() * MaxLockOnDist;
	FVector BoxHalfSize(100.f);
	MakeTraceBoxAndCheckHit(start, end, BoxHalfSize);
}

void UItemInventoryComponent::SpawnItem(const FItemTable itemData)
{
	// 아이템 스폰
	
}

void UItemInventoryComponent::MakeTraceBoxAndCheckHit(FVector start, FVector end, FVector boxHalfSize)
{
	FCollisionQueryParams Params;
	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		start,
		end,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeBox(boxHalfSize),
		Params);

	FColor BoxColor = FColor::Green;
	if (bHit)
	{
		for (const FHitResult hit : HitResults)
		{
			auto* lockedTarget = Cast<AKart>(hit.GetActor());
			if (lockedTarget)
			{
				BoxColor = FColor::Red;
				LockedTarget = lockedTarget;
			}
		}
	}
	else
	{
		BoxColor = FColor::Green;
		LockedTarget = nullptr;
	}

	int NumSteps = 10;
	for (int i = 0; i <= NumSteps; i++)
	{
		float Alpha = (float)i / NumSteps;
		FVector DebugLocation = FMath::Lerp(start, end, Alpha);
	
		DrawDebugBox(GetWorld(), DebugLocation, boxHalfSize, FQuat::Identity, BoxColor, false, 0.1f);
	}
	
}

