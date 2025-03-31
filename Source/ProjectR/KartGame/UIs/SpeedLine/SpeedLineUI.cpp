// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedLineUI.h"

#include "Kart.h"
#include "KartSystemLibrary.h"
#include "SpeedLinePart.h"

void USpeedLineUI::Init()
{
	SpeedLineParts.Add(WBP_SpeedLine1);
	SpeedLineParts.Add(WBP_SpeedLine2);
	SpeedLineParts.Add(WBP_SpeedLine3);
	SpeedLineParts.Add(WBP_SpeedLine4);

	for (int i = 0; i < SpeedLineParts.Num(); i++)
	{
		SpeedLineParts[i]->Init();
	}

	Kart = GetWorld()->GetFirstPlayerController()->GetPawn<AKart>();
	KartBody = Kart->GetRootBox();

	GetWorld()->GetTimerManager().SetTimer(LineShowTimer, this, &USpeedLineUI::ShowLines, LineShowTime, true);
}

void USpeedLineUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!Kart || !KartBody) return ;
	
	SpeedLineUICount = CalculateSpeedLineUICount();
}

int32 USpeedLineUI::CalculateSpeedLineUICount()
{
	int32 Result = DefaultSpeedLineCount;

	float NormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());
	// 정규화된 속도가 1.0 이상이면 MaxSpeedLineCount를 반환
	// 0.1 이하이면 DefaultSpeedLineCount를 반환
	// 0.0 이하이면 0을 반환
	if (NormalizedSpeed >= 1.0f)
	{
		Result = MaxSpeedLineCount;
	}
	else if (NormalizedSpeed > 0.5f)
	{
		Result = DefaultSpeedLineCount;
	}
	else
	{
		Result = 0;
	}
	
	return Result;
}

void USpeedLineUI::ShowLines()
{
	for (int32 i = 0; i < SpeedLineParts.Num(); i++)
	{
		SpeedLineParts[i]->Update(SpeedLineUICount);
	}
}

void USpeedLineUI::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(LineShowTimer);
}