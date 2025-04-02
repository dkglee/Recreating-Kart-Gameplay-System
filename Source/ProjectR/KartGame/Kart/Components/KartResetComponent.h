// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "KartResetComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartResetComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKartResetComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent);

private:
	UPROPERTY()
	class AKart* Kart = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Rollback = nullptr;
	
	UPROPERTY()
	TObjectPtr<class UTimelineComponent> RollbackEffectTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> RollbackEffectCurve;

	UPROPERTY()
	FOnTimelineFloat RollbackEffectCallback;
	UPROPERTY()
	FOnTimelineEvent RollbackEffectFinish;

	UFUNCTION()
	void OnRollbackInputDetected();
	
	UFUNCTION()
	void OnRollbackEffectCallback(const float Value);
	
	UFUNCTION()
	void OnRollbackEffectFinish();
};
