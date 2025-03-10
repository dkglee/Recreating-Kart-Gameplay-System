// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartSuspensionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartSuspensionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartSuspensionComponent();
	bool GetLandScapeNormal(FVector& OutNormam, FVector& OutLocation);
	bool ProcessSuspension();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	UPROPERTY()
	class AKart* Kart = nullptr;
	UPROPERTY()
	class UBoxComponent* KartBody = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension", meta = (AllowPrivateAccess = "true"))
	float ForceScale = 85000.0f * 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Suspension", meta = (AllowPrivateAccess = "true"))
	int SuspensionLength = 60.0f;

	bool bHitLandScape = false;
	FVector LandScapeNormal = FVector::ZeroVector;
	FVector LandScapeLocation = FVector::ZeroVector;
};
