// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent);
	// UFUNCTION()
	// void OnAccelerationInputDetected(float InAccelerationIntensity);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void ProcessSteeringAndTorque();

private:
	void ProcessSteering();
	void OnSteeringInputDetected(const FInputActionValue& InputActionValue);
	// Client, Server 둘 다 호출되어야 함. (클라이언트에서 호출)
	UFUNCTION(Server, Reliable)
	void ApplySteeringToKart(float InTargetSteering);
	// 서버에서 호출되어야 함. (클라이언트에서 호출)
	UFUNCTION(Server, Reliable)
	void ApplyTorqueToKartV2(float InSteering);

	UFUNCTION(Server, Reliable)
	void ApplyTorqueToKart(float InAccerlationIntensity, float InSteeringIntensity);
	// 바퀴가 어디로 움직였는지. 혹은 앞으로 계산할 때 해당 변수가 필요할 수도 있음.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Steering = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteeringIntensity = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteerRate = 6.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteerPower = 50000000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float DampingCoefficient = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float AccelerationIntensity = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float TargetSteering = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float MaxRotation = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UCurveFloat* SteeringCurve = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float TurnScaling = 300.0f;
};
