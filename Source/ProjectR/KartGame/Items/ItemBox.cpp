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
	TMap<int32, FItemTable> itemMap = ItemInventoryComponent->ItemMap;
	for (const auto& item : itemMap)
	{
		TotalWeight += item.Value.ItemWeight;
	}
	
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
	
	auto* ps = Cast<ARiderPlayerState>(player->GetPlayerState());
	if (ps)
	{
		FFastLogger::LogConsole(TEXT("Ranking : %d"), ps->GetRanking());

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

void AItemBox::RotateBody()
{
	float speed = 100.f;
	float newYaw = GetActorRotation().Yaw + speed * GetWorld()->GetDeltaSeconds();
	FRotator newRot(GetActorRotation().Pitch, newYaw, GetActorRotation().Roll);
	SetActorRotation(newRot);
}
