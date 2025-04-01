// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "KartBasicBoosterVFXComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartBasicBoosterVFXComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartBasicBoosterVFXComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

protected:
	UPROPERTY()
	class AKart* Kart = nullptr;
	
	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_ActivateBoosterVFX(float BoosterTime);
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_ActivateBoosterVFX(float BoosterTime);

	UFUNCTION(Server, Reliable)
	virtual void ServerRPC_DeactivateBoosterVFX();
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastRPC_DeactivateBoosterVFX();

	UFUNCTION()
	virtual void OnBoosterActivated(float BoosterTime);
	UFUNCTION()
	virtual void OnBoosterDeactivated();

	UPROPERTY()
	FTimerHandle TimerHandle;
};
