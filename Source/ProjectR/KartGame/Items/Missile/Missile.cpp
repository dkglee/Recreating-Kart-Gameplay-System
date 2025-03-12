// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
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
			FFastLogger::LogConsole(TEXT("Hit LockOnPlayer"));
			Destroy();
		}
	}
}

void AMissile::MovetoTarget(float DeltaTime)
{
	// TODO
	// 현재 목표위치에 도달햇지만 회전으로 인해 미사일이 맞지않고 계속 회전하는 현상이 있음
	// 타겟과의 거리에 따라서 파동의 높이와 주기를 줄이면 해결될 문제
	FVector p0 = GetActorLocation();
	FVector dir = (LockOnPlayer->GetActorLocation() - p0).GetSafeNormal();
	FVector vt = dir * speed * DeltaTime;
	FVector p = p0 + vt;

	ElapsedTime += DeltaTime;
	
	float SinValue = FMath::Sin(ElapsedTime * Frequency);
	float CosValue = FMath::Cos(ElapsedTime * Frequency);

	
	float YOffset = CosValue * Amplitude;
	float ZOffset = SinValue * Amplitude;
	
	p.Y = p0.Y + YOffset;
	p.Z = p0.Z + ZOffset;

	SetActorLocation(p);

	FRotator rot = GetActorRotation();
	FRotator targetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),LockOnPlayer->GetActorLocation());
	FRotator newRot = UKismetMathLibrary::RInterpTo(rot,targetRot,DeltaTime,1.0f);
	SetActorRotation(newRot);
}



