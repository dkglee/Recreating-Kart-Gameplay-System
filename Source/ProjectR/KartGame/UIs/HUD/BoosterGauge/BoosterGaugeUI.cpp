// Fill out your copyright notice in the Description page of Project Settings.


#include "BoosterGaugeUI.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartFrictionComponent.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"

void UBoosterGaugeUI::Init()
{
	Kart = GetWorld()->GetFirstPlayerController()->GetPawn<AKart>();;
	if (Kart)
	{
		Kart->GetFrictionComponent()->OnBoosterGaugeUpdated.AddDynamic(this, &UBoosterGaugeUI::OnBoosterGaugeUpdated);
	}
}

void UBoosterGaugeUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UBoosterGaugeUI::OnBoosterGaugeUpdated(float DriftGauge, float DriftGaugeMax)
{
	WidgetDriftGauge = DriftGauge;
	WidgetDriftGaugeMax = DriftGaugeMax;

	WidgetDriftGauge = FMath::Clamp(WidgetDriftGauge, 0.0f, WidgetDriftGaugeMax);

	// FFastLogger::LogConsole(TEXT("DriftGauge: %f, DriftGaugeMax: %f"), DriftGauge, DriftGaugeMax);
}
