// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartCameraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartCameraComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UKartCameraComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void OnBoosterActivated(float BoosterTime);
	UFUNCTION()
	void OnInstantBoosterActivated(float BoosterTime);
	UFUNCTION()
	void SetDefaultSpringArmLength();

	
	UPROPERTY()
	class AKart* Kart = nullptr;
	UPROPERTY()
	class UCameraComponent* KartCamera = nullptr;
	UPROPERTY()
	class USpringArmComponent* KartSpringArm = nullptr;
	UPROPERTY()
	class UKartBoosterComponent* KartBoosterComponent = nullptr;

	UPROPERTY()
	float DefaultSpringArmLength = 0.0f;
	UPROPERTY()
	float BoosterTargetSpringArmLegnth = 750.0f;
	UPROPERTY()
	float InstantTargetSpringArmLength = 600.0f;
	UPROPERTY()
	float TargetSpringArmLength = 0.0f;

	UPROPERTY()
	FTimerHandle CameraLerpTimer;
};
