// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpeedLinePart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API USpeedLinePart : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();
	void Update(int32 SpeedLineUICount);
	
private:
	void Clear();
	
	UPROPERTY(meta = (BindWidget))
	class UImage* Line1;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line2;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line3;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line4;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line5;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line6;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line7;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line8;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line9;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line10;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line11;
	UPROPERTY(meta = (BindWidget))
	class UImage* Line12;

	UPROPERTY()
	TArray<UImage*> SpeedLines;

	UPROPERTY()
	float MinOpacity = 0.1f;
	UPROPERTY()
	float MaxOpacity = 0.25f;
};
