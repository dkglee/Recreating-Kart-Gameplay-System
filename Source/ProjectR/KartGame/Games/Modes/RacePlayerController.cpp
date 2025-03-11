// Fill out your copyright notice in the Description page of Project Settings.


#include "RacePlayerController.h"

#include "Blueprint/UserWidget.h"

void ARacePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController())
	{
		UUserWidget* HUD = CreateWidget(this, MainHUDClass);
		HUD->AddToViewport();
	}
}