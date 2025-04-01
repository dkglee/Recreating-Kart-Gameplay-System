// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadReckoningStrategy.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameStateBase.h"

void UDeadReckoningStrategy::Initialize(class AKart* InKart)
{
	Super::Initialize(InKart);

	Kart = InKart;
	KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
}

void UDeadReckoningStrategy::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	// 서버 시간 기준 Delta 구하기
	float CurrentTime = Kart->GetWorld()->GetGameState()->GetServerWorldTimeSeconds();
	float Delta = CurrentTime - LastUpdatedKartInfo.TimeStamp;

	// Dead Reckoning 위치 예측
	PredictedPosition = LastUpdatedKartInfo.KartTransform.GetLocation() + LastUpdatedKartInfo.Velocity * Delta;

	// 회전은 보간 처리 (스무딩)
	PredictedRotation = FMath::RInterpTo(KartBody->GetComponentRotation(), LastUpdatedKartInfo.KartTransform.GetRotation().Rotator(), DeltaTime, 5.0f);

	FVector NewLocation = FMath::VInterpTo(Kart->GetActorLocation(), PredictedPosition, DeltaTime, 5.0f);

	if (FVector::Dist(Kart->GetActorLocation(), NewLocation) > 10.0f)
	{
		Kart->SetActorLocationAndRotation(NewLocation, PredictedRotation, false);
	}
	else
	{
		Kart->SetActorLocationAndRotation(NewLocation, PredictedRotation, true);
	}
}

void UDeadReckoningStrategy::UpdateRemoteState(const FKartInfo& NewKartInfo)
{
	Super::UpdateRemoteState(NewKartInfo);

	LastUpdatedKartInfo = NewKartInfo;
}
