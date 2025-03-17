// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashBoardUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UDashBoardUI : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetDashBoardValue(float CurrentSpeed, float MaxSpeed);
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URadialSlider> DashBoard_Back;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DashBoard_Text;
};
