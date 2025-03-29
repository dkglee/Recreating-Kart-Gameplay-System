// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedLinePart.h"

#include "Components/Image.h"

void USpeedLinePart::Init()
{
	SpeedLines.Add(Line1);
	SpeedLines.Add(Line2);
	SpeedLines.Add(Line3);
	SpeedLines.Add(Line4);
	SpeedLines.Add(Line5);
	SpeedLines.Add(Line6);
	SpeedLines.Add(Line7);
	SpeedLines.Add(Line8);
	SpeedLines.Add(Line9);
	SpeedLines.Add(Line10);
	SpeedLines.Add(Line11);
	SpeedLines.Add(Line12);

	Clear();
}

void USpeedLinePart::Update(int32 SpeedLineUICount)
{
	Clear();
	// SpeedLineUICount만큼 랜덤으로 SpeedLine을 활성화
	TMap<int32, bool> RandomIndexMap;
	while (RandomIndexMap.Num() < SpeedLineUICount) {
		int32 RandomIndex = FMath::RandRange(0, SpeedLines.Num() - 1);
		if (RandomIndexMap.Contains(RandomIndex))
		{
			continue;
		}
		RandomIndexMap.Add(RandomIndex, true);
		SpeedLines[RandomIndex]->SetRenderOpacity(FMath::RandRange(MinOpacity, MaxOpacity));
	}
}

void USpeedLinePart::Clear()
{
	for (int i = 0; i < SpeedLines.Num(); i++)
	{
		SpeedLines[i]->SetRenderOpacity(0.0f);
	}
}
