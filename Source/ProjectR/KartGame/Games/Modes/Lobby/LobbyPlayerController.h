// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class UMainLobbyUI;
/**
 * 
 */
UCLASS()
class PROJECTR_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void ActiveLobbyUI();
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainLobbyUI> MainLobbyUIClass;
	
	UPROPERTY()
	TObjectPtr<UMainLobbyUI> MainLobbyUI;
};
