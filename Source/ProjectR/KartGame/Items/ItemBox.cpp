// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "Components/ItemInventoryComponent.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

AItemBox::AItemBox()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	Super::SetReplicateMovement(true);

	InitComponents();
}

void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
	Root->OnComponentBeginOverlap.AddDynamic(this,&AItemBox::ItemBoxBeginOverlap);
}

void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateBody();
}

void AItemBox::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearTimer(ItemBoxRespawnTimerHandle);
}

void AItemBox::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetGenerateOverlapEvents(true);
	Root->SetBoxExtent(FVector(75.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void AItemBox::ItemBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false) return;
	auto* player = Cast<AKart>(OtherActor);
	if (player)
	{
		MakeRandomItem(player->GetItemInventoryComponent(), player);
	}
}

void AItemBox::MakeRandomItem(class UItemInventoryComponent* ItemInventoryComponent, class AKart* player)
{
	Server_MakeRandomItem(ItemInventoryComponent, player);
}

void AItemBox::Server_MakeRandomItem_Implementation(class UItemInventoryComponent* ItemInventoryComponent, class AKart* player)
{
	auto* ps = Cast<ARiderPlayerState>(player->GetPlayerState());
	TMap<int32, FItemTable> itemMap = ItemInventoryComponent->ItemMap;
	int32 playerRank = ps->GetRanking();
	
	ApplyRankBasedWeightAdjustments(itemMap, playerRank);

	int32 TotalWeight = CalculateTotalWeight(itemMap);
	int32 RandomValue = FMath::RandRange(1,TotalWeight - 1);
	FFastLogger::LogConsole(TEXT("RandomWeight : %d"), RandomValue);
	
	int32 CurrentWeight = 0;
	FItemTable RandomItem = {};
	
	for (const auto& Item : itemMap)
	{
		CurrentWeight += Item.Value.ItemWeight;
		FFastLogger::LogConsole(TEXT("CurrentWeight : %d"), CurrentWeight);
		if (RandomValue < CurrentWeight)
		{
			RandomItem = Item.Value;
			break;
		}
	}

	if (ps->GetRanking() == 1)
	{
		while (RandomItem.ItemID < 10)
		{
			RandomValue = FMath::RandRange(1,TotalWeight - 1);
			FFastLogger::LogConsole(TEXT("(부스터여서 다시뽑기)RandomWeight : %d"), RandomValue);
			CurrentWeight = 0;
			
			for (const auto& Item : itemMap)
			{
				CurrentWeight += Item.Value.ItemWeight;
				if (RandomValue < CurrentWeight)
				{
					RandomItem = Item.Value;
					break;
				}
			}
		}
	}
	
	NetMultiCast_MakeRandomItem(ItemInventoryComponent, RandomItem);
}

void AItemBox::NetMultiCast_MakeRandomItem_Implementation(class UItemInventoryComponent* ItemInventoryComponent, const FItemTable Item)
{
	//FFastLogger::LogConsole(TEXT("IsServer: %s, Role: %d"), HasAuthority() ? TEXT("True") : TEXT("False"), GetLocalRole());
	
	if (HasAuthority())
	{
		ItemInventoryComponent->GetItem(Item);
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	if (GetWorldTimerManager().TimerExists(ItemBoxRespawnTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(ItemBoxRespawnTimerHandle);
	}

	TWeakObjectPtr<AItemBox> weakThis = this;
	GetWorldTimerManager().SetTimer(ItemBoxRespawnTimerHandle,  [weakThis]()
	{
		if (weakThis.IsValid())
		{
			weakThis->SetActorHiddenInGame(false);
			weakThis->SetActorEnableCollision(true);
		}
	}, 5.f, false);
}

void AItemBox::ApplyRankBasedWeightAdjustments(TMap<int32, FItemTable>& ItemMapToAdjust, int32 PlayerRank)
{
	const int32 AttackItemMinID = 10;
	const int32 AttackItemMaxID = 19;
    
	const int32 DefenseItemMinID = 20;
	const int32 DefenseItemMaxID = 29;
    
	const int32 BoosterItemMinID = 1;
	const int32 BoosterItemMaxID = 9;
    
	const int32 MaxPlayers = GetWorld()->GetNumControllers();
	
	float NormalizedRank = (float)PlayerRank / MaxPlayers;

	float BoosterBoostFactor = FMath::Clamp(NormalizedRank, 0.0f, 1.0f);
	float AttackBoostFactor = 1.0f - 4.0f * FMath::Pow(NormalizedRank - 0.5f, 2);
	float DefenseBoostFactor = FMath::Clamp(1.0f - ((float)PlayerRank - 1) / (MaxPlayers - 1), 0.f, 1.f);

	for (auto& Item : ItemMapToAdjust)
	{
		int32 ItemID = Item.Value.ItemID;
		// 기본 가중치
		float WeightMultiplier = 1.0f;

		if (ItemID >= AttackItemMinID && ItemID <= AttackItemMaxID)
		{
			WeightMultiplier = 1.0f + AttackBoostFactor;
		}
		else if (ItemID >= DefenseItemMinID && ItemID <= DefenseItemMaxID)
		{
			WeightMultiplier = 1.0f + DefenseBoostFactor;
		}
		else if (ItemID >= BoosterItemMinID && ItemID <= BoosterItemMaxID)
		{
			WeightMultiplier = 1.0f + BoosterBoostFactor;
		}

		Item.Value.ItemWeight = FMath::FloorToInt(Item.Value.ItemWeight * WeightMultiplier);

		Item.Value.ItemWeight = FMath::Max(Item.Value.ItemWeight, 1);
	}
}

int32 AItemBox::CalculateTotalWeight(const TMap<int32, FItemTable>& ItemMapToCalculate)
{
	int32 Total = 0;
	for (const auto& Item : ItemMapToCalculate)
	{
		Total += Item.Value.ItemWeight;
	}
	return Total + 1;
}

void AItemBox::RotateBody()
{
	float speed = 100.f;
	float newYaw = GetActorRotation().Yaw + speed * GetWorld()->GetDeltaSeconds();
	FRotator newRot(GetActorRotation().Pitch, newYaw, GetActorRotation().Roll);
	SetActorRotation(newRot);
}
