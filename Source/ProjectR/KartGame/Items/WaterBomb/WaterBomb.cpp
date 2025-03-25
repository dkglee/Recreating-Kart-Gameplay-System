// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBomb.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "KartGame/Items/Components/ItemInteractionComponent.h"


// Sets default values
AWaterBomb::AWaterBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Super::SetReplicateMovement(true);
	Root->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AWaterBomb::BeginPlay()
{
	Super::BeginPlay();

	Root->OnComponentBeginOverlap.AddDynamic(this, &AWaterBomb::OnWaterBombBeginOverlap);
}

// Called every frame
void AWaterBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaterBomb::OnWaterBombBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false) return;
	
	auto* Kart = Cast<AKart>(OtherActor);
	if (Kart == GetOwningPlayer())
	{
		FFastLogger::LogConsole(TEXT("Hit Kart is itemOwner"));
		return;
	}
	
	if (Kart)
	{
		// 이미 물풍선에 맞았으면 return;
		if (Kart->GetItemInteractionComponent()->GetbIsInteraction() == true &&
			Kart->GetItemInteractionComponent()->GetCurrentType() == EInteractionType::Water)
		{
			FFastLogger::LogConsole(TEXT("This actor is already interact"));
			return;
		}
		Kart->GetItemInteractionComponent()->Interaction(EInteractionType::Water);
	}
}

