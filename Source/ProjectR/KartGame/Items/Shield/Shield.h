// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KartGame/Items/BaseItem.h"
#include "Shield.generated.h"

UCLASS()
class PROJECTR_API AShield : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShield();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
