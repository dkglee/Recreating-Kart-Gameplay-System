// Fill out your copyright notice in the Description page of Project Settings.


#include "DashBoardUI.h"

#include "FastLogger.h"
#include "Components/RadialSlider.h"
#include "Components/TextBlock.h"

void UDashBoardUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDashBoardUI::SetDashBoardValue(float CurrentSpeed, float MaxSpeed)
{
	float Rate = FMath::Abs(CurrentSpeed) / MaxSpeed;
	DashBoard_Back->SetValue(Rate);

	// cm/s -> m/s -> km/h
	float KartSpeedKM = FMath::Abs(CurrentSpeed) * 0.036;
	// Km/h
	int32 Speed = FMath::RoundToInt(KartSpeedKM);

	// 세 자리수로 맞춤 (예: 032, 005, 120)
	DashBoard_Text->SetText(FText::FromString(FString::Printf(TEXT("%03d"), Speed)));
}
