// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KartGame/Items/BaseItem.h"
#include "WaterBomb.generated.h"

UCLASS()
class PROJECTR_API AWaterBomb : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaterBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
