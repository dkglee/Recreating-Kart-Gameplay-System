// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API ARacePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> MainHUDClass;
};
