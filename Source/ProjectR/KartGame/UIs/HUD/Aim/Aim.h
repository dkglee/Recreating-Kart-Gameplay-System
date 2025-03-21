// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Aim.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UAim : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* AimImage;

	UPROPERTY(EditAnywhere)
	class UTexture2D* GreenAim;

	UPROPERTY(EditAnywhere)
	class UTexture2D* RedAim;
};
