// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UItemInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemInteractionComponent();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void MissileHitInteraction();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	
public:
	FTimerHandle KnockbackTimerHandle;
	
	float KnockbackElapsedTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float KnockbackTime = 1.5f;

	bool bIsInteraction = false;

	FQuat InitialQuat;
	
};
