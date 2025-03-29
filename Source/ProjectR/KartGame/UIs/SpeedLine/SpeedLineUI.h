// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpeedLineUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API USpeedLineUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	void ClearTimer();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	UFUNCTION()
	int32 CalculateSpeedLineUICount();
	UFUNCTION()
	void ShowLines();

	UPROPERTY(meta = (BindWidget))
	class USpeedLinePart* WBP_SpeedLine1;
	UPROPERTY(meta = (BindWidget))
	class USpeedLinePart* WBP_SpeedLine2;
	UPROPERTY(meta = (BindWidget))
	class USpeedLinePart* WBP_SpeedLine3;
	UPROPERTY(meta = (BindWidget))
	class USpeedLinePart* WBP_SpeedLine4;

	UPROPERTY()
	TArray<USpeedLinePart*> SpeedLineParts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpeedLineUI", meta = (AllowPrivateAccess = "true"))
	int32 MaxSpeedLineCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpeedLineUI", meta = (AllowPrivateAccess = "true"))
	int32 DefaultSpeedLineCount = 1;

	UPROPERTY()
	class AKart* Kart;
	UPROPERTY()
	class UBoxComponent* KartBody;

	UPROPERTY()
	FTimerHandle LineShowTimer;
	UPROPERTY()
	float LineShowTime = 0.15f;

	int32 SpeedLineUICount = 0;
};
