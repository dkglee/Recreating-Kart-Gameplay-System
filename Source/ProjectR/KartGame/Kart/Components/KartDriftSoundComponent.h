// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "KartDriftSoundComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartDriftSoundComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartDriftSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	
public:
	UFUNCTION()
	void PlayDriftSound(bool bDrift);
	
private:
#pragma region CachedVariable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UKartNetworkSyncComponent* KartNetworkSyncComponent = nullptr;
#pragma endregion

	
#pragma region CoreSound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* DriftSoundSource = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Sound", meta = (AllowPrivateAccess = "true"))
	class USoundAttenuation* DriftSoundAttenuation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Sound", meta = (AllowPrivateAccess = "true"))
	float MaxDriftVolume = 2.0f;
#pragma endregion
};
