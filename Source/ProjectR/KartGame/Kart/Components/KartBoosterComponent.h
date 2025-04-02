// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Components/ActorComponent.h"
#include "KartBoosterComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoosterActivated, float, BoosterTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBoosterDeactivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInstantBoosterActivated, float, BoosterTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInstantBoosterDeactivated);


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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; 
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
	UFUNCTION()
	void ApplyInstantBoost();
	UFUNCTION(Server, Reliable)
	void ServerRPC_SetbOnBooster(bool bInOnBooster);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetbOnBooster(bool bInOnBooster);

public:
	void ProcessBooster(bool bBoosterUsing);

	GETTER(bool, bOnBooster);

	UPROPERTY()
	FOnBoosterActivated OnBoosterActivated;
	UPROPERTY()
	FOnBoosterDeactivated OnBoosterDeactivated;
	UPROPERTY()
	FOnInstantBoosterActivated OnInstantBoosterActivated;
	UPROPERTY()
	FOnInstantBoosterDeactivated OnInstantBoosterDeactivated;
	
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	class UKartAccelerationComponent* AccelerationComponent = nullptr;

#pragma region BoosterVariance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	float BoosterForce = 1750.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Booster", meta = (AllowPrivateAccess = "true"))
	float BoosterTime = 3.f;

	UPROPERTY()
	float ElapsedTime = 0.f;
#pragma endregion 

#pragma region InstantBoosterVariance
	UPROPERTY()
	bool bInstantBoostEnabled = false;

	UPROPERTY()
	FTimerHandle InstantBoostTimer;
	UPROPERTY()
	float InstantBoostDuration = 1.5f;
	UPROPERTY()
	float InstantBoostScale = 1.0f;

	UPROPERTY()
	FTimerHandle InstantBoostActiveTimer;
	UPROPERTY()
	bool bInstantBoostActive = false;
	UPROPERTY()
	float InstantBoostActiveDuration = 0.5f;
#pragma endregion
	
	UPROPERTY()
	bool bOnBooster = false;
};
