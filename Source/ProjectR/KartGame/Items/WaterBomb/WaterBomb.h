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
	AWaterBomb();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnWaterBombBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
};
