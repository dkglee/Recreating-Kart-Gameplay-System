// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "KartSteeringComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartSteeringComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartSteeringComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void ProcessSteering();
	void ProcessTorque(bool bDrift);

#pragma region GetterSetters
	GETTER(float, SteeringIntensity);
	GETTER(float, AccelerationIntensity);
	GETTER(float, TargetSteering);
	GETTER(float, MaxRotation);
	//장진혁 추가
	GETTER(float, SteeringInput);
#pragma endregion
private:
	UFUNCTION()
	void OnDriftKeyEdged(bool bDriftInput);
	void OnSteeringInputDetected(const FInputActionValue& InputActionValue);
	// Client, Server 둘 다 호출되어야 함. (클라이언트에서 호출)
	UFUNCTION(Server, Reliable)
	void ApplySteeringToKart(float InTargetSteering);
	// 서버에서 호출되어야 함. (클라이언트에서 호출)
	UFUNCTION(Server, Reliable)
	void ApplyTorqueToKartV2(float InSteering, bool bDrfit);
	float CalculateNewTurnScale(bool bDrift);
	// 장진혁 추가
	UFUNCTION(Server, Reliable)
	void Server_GetInputSteering(float value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Steering = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteeringIntensity = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteerRate = 3.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float DampingCoefficient = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float AccelerationIntensity = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float TargetSteering = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float MaxRotation = 25.0f;

	// 장진혁 추가
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteeringInput = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* SteeringCurve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float TurnScaling = 350.0f;

	UPROPERTY()
	bool bWasDrifting = false;
	UPROPERTY()
	float DriftBoostTimer = 0.0f;
	UPROPERTY()
	float DriftBoostDuration = 0.25f;
	UPROPERTY()
	bool bDriftKeyEdged = false;
};
