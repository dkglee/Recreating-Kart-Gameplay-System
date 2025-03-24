// Fill out your copyright notice in the Description page of Project Settings.


#include "Booster.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartBoosterComponent.h"


// Sets default values
ABooster::ABooster()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void ABooster::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABooster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 서버에서 처리하도록 처리
	if (HasAuthority() == false) return;

	// Kart는 해당 부스터를 소유하던 카트의 포인터이다.
	auto* Kart = GetOwningPlayer();
	if (Kart)
	{
		FFastLogger::LogConsole(TEXT("BoosterActor) IsServer: %s, Role: %d"), Kart->HasAuthority() ? TEXT("True") : TEXT("False"), Kart->GetLocalRole());
		if (Kart->GetbUsingBooster() == false)
		{
			Kart->SetbUsingBooster(true);
		}
		else
		{
			ElapsedTime += DeltaTime;
			if (ElapsedTime >= BoosterTime)
			{
				Kart->SetbUsingBooster(false);
				this->Destroy();
			}
		}
	}
}

