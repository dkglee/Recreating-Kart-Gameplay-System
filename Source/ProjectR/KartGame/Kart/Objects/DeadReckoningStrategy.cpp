// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadReckoningStrategy.h"
#include "Kart.h"

void UDeadReckoningStrategy::Initialize(class AKart* InKart)
{
	Super::Initialize(InKart);

	Kart = InKart;
}

void UDeadReckoningStrategy::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	// 현재 시간 기준 DeltaTime
	float CurrentTime = Kart->GetWorld()->GetTimeSeconds();
}

void UDeadReckoningStrategy::UpdateRemoteState(const FKartInfo& NewKartInfo)
{
	Super::UpdateRemoteState(NewKartInfo);

	LastUpdatedKartInfo = NewKartInfo;
	// TODO : Dead Reckoning을 위한 네트워크 시간을 만들자!
	// Kart->GetWorld()->GetFirstPlayerController()->
}
