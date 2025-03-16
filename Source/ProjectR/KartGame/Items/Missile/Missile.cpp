// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "KartGame/Items/Components/ItemInteractionComponent.h"
#include "Kismet/KismetMathLibrary.h"


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
			kart->GetItemInteractionComponent()->MissileHitInteraction();
			Destroy();
		}
	}
}

void AMissile::MovetoTarget(float DeltaTime)
{
	DistanceToTarget = FVector::Dist(GetActorLocation(), LockOnPlayer->GetActorLocation());
	
	FVector p0 = GetActorLocation();
	FVector dir = (LockOnPlayer->GetActorLocation() - p0).GetSafeNormal();
	float speedFactor = FMath::Clamp(DistanceToTarget / 100.0f, 0.5f, 2.0f);
	FVector vt = dir * speed * speedFactor * DeltaTime;

	ElapsedTime += DeltaTime;
	float DistanceFactor = FMath::Clamp(DistanceToTarget / 1000.0f, 0.1f, 1.0f);
	float YOffset = FMath::Cos(ElapsedTime * Frequency) * Amplitude * DistanceFactor;
	float ZOffset = FMath::Sin(ElapsedTime * Frequency) * Amplitude * DistanceFactor;

	FVector p = p0 + vt;
	p.Y += YOffset;
	p.Z += ZOffset;
	
	SetActorLocation(p);

	FRotator rot = GetActorRotation();
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),LockOnPlayer->GetActorLocation());
	FRotator newRot = UKismetMathLibrary::RInterpTo(rot,targetRot,DeltaTime,1.0f);
	SetActorRotation(newRot);
}



