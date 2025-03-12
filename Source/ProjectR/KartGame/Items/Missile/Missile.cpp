// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"


// Sets default values
AMissile::AMissile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMissile::BeginPlay()
{
	Super::BeginPlay();

	Root->OnComponentBeginOverlap.AddDynamic(this,&AMissile::OnMissileBeginOverlap);
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovetoTarget(DeltaTime);
	
}

void AMissile::OnMissileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto* kart = Cast<AKart>(OtherActor);
	if (kart)
	{
		if (kart == LockOnPlayer)
		{
			FFastLogger::LogConsole(TEXT("Hit LockOnPlayer"));
			Destroy();
		}
	}
}

void AMissile::MovetoTarget(float DeltaTime)
{
	FVector p0 = GetActorLocation();
	FVector dir = (LockOnPlayer->GetActorLocation() - p0).GetSafeNormal();
	FVector vt = dir * speed * DeltaTime;
	SetActorLocation(p0 + vt);

	
}



