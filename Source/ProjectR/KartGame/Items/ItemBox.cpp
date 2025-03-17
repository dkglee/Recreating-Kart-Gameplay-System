// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"

#include "Kart.h"
#include "Components/BoxComponent.h"
#include "Components/ItemInventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectR/ProjectR.h"
#include "ProjectR/KartGame/Games/KartGameInstance.h"

AItemBox::AItemBox()
{
	PrimaryActorTick.bCanEverTick = true;

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
}

void AItemBox::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetGenerateOverlapEvents(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Items = LoadObject<UDataTable>(nullptr,TEXT("'/Game/Items/DataTable/ItemTable.ItemTable'"));
	if (Items)
	{
		TArray<FName> RowNames = Items->GetRowNames();

		for (const FName& RowName : RowNames)
		{
			FItemTable* Row = Items->FindRow<FItemTable>(RowName, TEXT(""));
			if (Row != nullptr)
			{
				TotalWeight += Row->ItemWeight;
				ItemMap.Add(Row->ItemID,Row);
			}
		}
	}
}

void AItemBox::ItemBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* player = Cast<AKart>(OtherActor);
	if (player)
	{
		MakeRandomItem(player->GetItemInventoryComponent());
	}
}

void AItemBox::MakeRandomItem(class UItemInventoryComponent* ItemInventoryComponent)
{
	int32 RandomValue = FMath::RandRange(1,TotalWeight-1);
	int32 CurrentWeight = 0;
	
	for (const auto& Item : ItemMap)
	{
		CurrentWeight += Item.Value->ItemWeight;
		if (RandomValue < CurrentWeight)
		{
			ItemInventoryComponent->GetItem(Item.Value);
			return;	
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("데이터가 이상해요"));
}

