// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectR.h"
#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "KartSteeringComponentLegacy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSteeringDelegate, float, SteeringAngle);

/*
 * 차량의 조향각(steering angle)과 회전 속도를 제어.
 * 조향 입력(좌/우)을 받아 조향각을 점진적으로 변화시킴.
 * 속도와 무관하게 차량의 방향을 변경할 수 있도록 설계.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartSteeringComponentLegacy : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartSteeringComponentLegacy();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FOnSteeringDelegate OnSteeringDelegate;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInput);

private:
	// 이동 처리
	void OnSteeringInputDetected(const FInputActionValue& InputActionValue);
	void ProcessSteering();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Steering = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteeringInput = 0.0f;  // 입력값 (-1 ~ 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float Steer = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteeringAngle = 0.0f;  // 현재 조향각
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float SteeringRate = 5.0f;  // 조향 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	float MaxSteeringAngle = 30.0f;  // 최대 조향각
};
