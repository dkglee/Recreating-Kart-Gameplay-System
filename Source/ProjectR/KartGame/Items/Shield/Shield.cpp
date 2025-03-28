// Fill out your copyright notice in the Description page of Project Settings.


#include "Shield.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartGame/Games/Modes/Race/RacePlayerController.h"
#include "KartGame/Items/Components/ItemInteractionComponent.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/NotificationTextUI/NotificationTextUI.h"


// Sets default values
AShield::AShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority() == false) return;

	auto* Kart = GetOwningPlayer();
	if (Kart)
	{
		Kart->GetItemInteractionComponent()->bShieldOn = true;
		auto* pc = Cast<ARacePlayerController>(Kart->GetController());
		if (pc)
		{
			pc->GetMainHUD()->GetWBP_NotificationTextUI()->SetShieldTextVisible(true);
		}
		FFastLogger::LogConsole(TEXT("shield on"));
		this->Destroy();
	}
}

