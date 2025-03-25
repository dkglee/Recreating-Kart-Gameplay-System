// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KartAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UKartAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UKartAnimInstance();

protected:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UFUNCTION()
	void ProcessRotateWheel();
	UFUNCTION()
	void ProcessBoosterState();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart AnimInstance", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart AnimInstance", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart AnimInstance", meta = (AllowPrivateAccess = "true"))
	float Accel = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart AnimInstance", meta = (AllowPrivateAccess = "true"))
	float Rotation = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart AnimInstance", meta = (AllowPrivateAccess = "true"))
	bool bBoosterUsing = false;
};
