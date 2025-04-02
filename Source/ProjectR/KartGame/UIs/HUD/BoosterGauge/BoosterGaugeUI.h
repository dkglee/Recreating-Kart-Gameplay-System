// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoosterGaugeUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UBoosterGaugeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UFUNCTION()
	void OnBoosterGaugeUpdated(float DriftGauge, float DriftGaugeMax);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AKart* Kart;

	UPROPERTY()
	float WidgetDriftGauge = 0.0f;
	UPROPERTY()
	float WidgetDriftGaugeMax = 100.0f;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* BoosterGauge;
};