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

private:
	void AddBoosterForce();

	UFUNCTION(Server, Reliable)
	void Server_AddBoosterForce();

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
	float BoosterForce = 1500.f;
	
};
