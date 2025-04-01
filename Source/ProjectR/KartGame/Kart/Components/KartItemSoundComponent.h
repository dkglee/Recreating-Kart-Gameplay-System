// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Components/AudioComponent.h"
#include "KartItemSoundComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartItemSoundComponent : public UAudioComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartItemSoundComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PlayShieldSound();
	void PlayWaterBombThrowSound();
	void PlayGetItemSound();
	void PlayMissileMoveSound();
	void PlayMissileHitSound();
	void PlayMissileFindTargetSound();
	void PlayMissileLockOnSound();
	void PlayAimMissSound();

	GETTER(USoundBase*, MissileLockOnSoundSource);
	GETTER(USoundBase*, MissileFindTargetSoundSource);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* ShieldSoundSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* WaterBombThrowSoundSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* GetItemSoundSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* MissileMoveSoundSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* MissileHitSoundSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* MissileLockOnSoundSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* MissileFindTargetSoundSource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Sound", meta = (AllowPrivateAccess = "true"))
	class USoundBase* AimMissSoundSource;
};
