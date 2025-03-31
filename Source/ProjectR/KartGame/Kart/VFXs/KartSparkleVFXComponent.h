// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "KartSparkleVFXComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartSparkleVFXComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartSparkleVFXComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void ActivateSparkleVFX();
	UFUNCTION()
	void DeactivateSparkleVFX();

	UFUNCTION(Server, Reliable)
	void ServerRPC_ActivateSparkleVFX();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_ActivateSparkleVFX();
	UFUNCTION(Server, Reliable)
	void ServerRPC_DeactivateSparkleVFX();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_DeactivateSparkleVFX();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sparkle", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
};
