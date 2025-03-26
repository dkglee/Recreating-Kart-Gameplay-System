// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "KartBoosterVFXComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartBoosterVFXComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartBoosterVFXComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

private:
	UPROPERTY()
	class AKart* Kart = nullptr;
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_ActivateBoosterVFX(float BoosterTime, bool bPowerBoost);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ActivateBoosterVFX(float BoosterTime, bool bPowerBoost);

	UFUNCTION(Server, Reliable)
	void ServerRPC_DeactivateBoosterVFX();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DeactivateBoosterVFX();

	UFUNCTION()
	void OnBoosterActivated(float BoosterTime, bool bPowerBoost);
	UFUNCTION()
	void OnBoosterDeactivated();

	FVector InstantBoostLength{100, 0, 0};
	FVector PowerBoostLength{150, 0, 0};
};
