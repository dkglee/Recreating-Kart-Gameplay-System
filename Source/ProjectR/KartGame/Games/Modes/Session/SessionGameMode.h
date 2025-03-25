// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SessionGameMode.generated.h"

UCLASS()
class PROJECTR_API ASessionGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void StartGameToPlay();

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
};
