// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KartGame/Items/BaseItem.h"
#include "Booster.generated.h"

UCLASS()
class PROJECTR_API ABooster : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABooster();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
};
