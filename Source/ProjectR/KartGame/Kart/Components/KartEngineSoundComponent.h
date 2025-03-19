// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "KartEngineSoundComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartEngineSoundComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartEngineSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	void PlayKartEngineSound();

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
	class USoundBase* EngineSoundSource = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Sound", meta = (AllowPrivateAccess = "true"))
	class USoundAttenuation* EngineSoundAttenuation = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Sound", meta = (AllowPrivateAccess = "true"))
	float MaxEngineRPM = 8000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Sound", meta = (AllowPrivateAccess = "true"))
	float CurrentEngineRPM = 0.0f;
#pragma endregion
};
