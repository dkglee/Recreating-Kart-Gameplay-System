// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "KartFrictionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDriftEnded);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartFrictionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartFrictionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ProcessFriction();

	void RollbackFriction();

#pragma region GetterSetters
	GETTER(bool, bDrift);
	GETTER_SETTER(float, InFrictionGripCoeff);
#pragma endregion
	
	UPROPERTY()
	FOnDriftEnded OnDriftEnded;
private:
	void OnDriftInputDetected(const FInputActionValue& InputActionValue);
	UFUNCTION(Server, Reliable)
	void ApplyFrictionToKart(bool bInDrift);
	void SetAngularDampling();
	void DetermineDriftState();
	void BroadCastDriftEnd();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Drift = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	bool bDriftInput = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	bool bDrift = false;
	UPROPERTY()
	bool bPrevDrift = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	float FrictionGrip = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* FrictionCurve = nullptr;

#pragma region Friction Rollback
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* FrictionRollbackCurve = nullptr;

	UPROPERTY()
	TObjectPtr<class UTimelineComponent> FrictionRollbackTimeline;

	FOnTimelineFloat FrictionRollbackCallback;

	FOnTimelineEvent FrictionRollbackFinish;
	
	UFUNCTION()
	void OnFrictionRollbackCallback(const float Value);

	UFUNCTION()
	void OnFrictionRollbackFinish();
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	float NormalAngularDamping = 0.05f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	float HardDriftAngularDamping = 0.01f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	float DefaultAngularDamping = 3.5f;

	float InFrictionData = 0.0f;
	// 마찰력 계수 Percent Gage
	float InFrictionGripCoeff = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	float MinimumFrictionDelay = 0.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Friction", meta = (AllowPrivateAccess = "true"))
	FTimerHandle FrictionDelayTimer;
	float bForceDrfit = false; 
};
