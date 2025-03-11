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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
