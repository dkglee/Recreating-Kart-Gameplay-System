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

	Items = LoadObject<UDataTable>(nullptr,TEXT("/Script/Engine.DataTable'/Game/Items/DataTable/ItemTable.ItemTable'"));
	if (Items)
	{
		ItemRowNames = Items->GetRowNames();
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
	int32 randomIdx = UKismetMathLibrary::RandomIntegerInRange(0,ItemRowNames.Num()-1);

	RandomItemData = *(Items->FindRow<FItemTable>(ItemRowNames[randomIdx],ItemRowNames[randomIdx].ToString()));
	UE_LOG(LogTemp, Warning, TEXT("Get Item : %s"), *RandomItemData.ItemName.ToString());

	ItemInventoryComponent->GetItem(RandomItemData);
}

