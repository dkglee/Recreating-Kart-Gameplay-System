// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "Components/ItemInventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectR/ProjectR.h"
#include "ProjectR/KartGame/Games/KartGameInstance.h"

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

void AItemBox::InitComponents()
{
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Root->SetGenerateOverlapEvents(true);
	Root->SetBoxExtent(FVector(75.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	UDataTable* Items = LoadObject<UDataTable>(nullptr,TEXT("'/Game/Items/DataTable/ItemTable.ItemTable'"));
	if (Items)
	{
		TArray<FName> RowNames = Items->GetRowNames();

		for (const FName& RowName : RowNames)
		{
			FItemTable* Row = Items->FindRow<FItemTable>(RowName, TEXT(""));
			if (Row != nullptr)
			{
				TotalWeight += Row->ItemWeight;
				ItemMap.Add(Row->ItemID,*Row);
			}
		}
	}
}

void AItemBox::ItemBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false) return;
	auto* player = Cast<AKart>(OtherActor);
	if (player)
	{
		MakeRandomItem(player->GetItemInventoryComponent());
	}
}

void AItemBox::MakeRandomItem(class UItemInventoryComponent* ItemInventoryComponent)
{
	Server_MakeRandomItem(ItemInventoryComponent);
}

void AItemBox::Server_MakeRandomItem_Implementation(class UItemInventoryComponent* ItemInventoryComponent)
{
	int32 RandomValue = FMath::RandRange(1,TotalWeight-1);
	int32 CurrentWeight = 0;
	
	for (const auto& Item : ItemMap)
	{
		CurrentWeight += Item.Value.ItemWeight;
		if (RandomValue < CurrentWeight)
		{
			NetMultiCast_MakeRandomItem(ItemInventoryComponent, Item.Value);
			return;	
		}
	}
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
