// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartBoosterComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartBoosterComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartBoosterComponent();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION(Server, Reliable)
	void Server_AddBoosterForce();
	UFUNCTION(Server, Reliable)
	void Server_ChangebUsingBooster(bool value);
	UFUNCTION()
	void ProcessInstantBoost();
	UFUNCTION()
	void EnableBoostWindow();

public:
	void ProcessBooster(bool bBoosterUsing);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	class UKartAccelerationComponent* AccelerationComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	float BoosterForce = 1750.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	float BoosterTime = 3.f;
	
	float ElapsedTime = 0.f;
	

	UPROPERTY()
	bool bInstantBoostEnabled = false;

	UPROPERTY()
	FTimerHandle InstantBoostTimer;
	UPROPERTY()
	float InstantBoostDuration = 1.5f;
	UPROPERTY()
	float InstantBoostScale = 0.5f;
};
