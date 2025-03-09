// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "KartMovementComponent.generated.h"

/*
 * 차량의 속도, 가속, 감속, 중력, 마찰력 등을 관리.
 * 이동 입력(전진, 후진)과 속도 변화를 처리.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartMovementComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent);

private:
	// 조향 처리 : Steering Delegate에 등록될 함수
	UFUNCTION()
	void UpdateSteeringAngle(float SteeringAngle);
	
#pragma region Movement
	// 이동 처리
	void OnMovementInputDetected(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void ProcessMovement();
	// Throttle : 가속도 (0.0f ~ 1.0f) : Player의 입력값에 영향을 받음
	UFUNCTION()
	void UpdateThrottle();
	// Brake : 감속도 (0.0f ~ 1.0f) : Player의 입력값에 영향을 받음
	UFUNCTION()
	void UpdateBrake();
	// F_drive : 차량의 종방향 힘 : Throttle에 영향을 받음
	UFUNCTION()
	void UpdateForceDrive();
	// F_brake : 차량의 종방향 감속 힘 : Brake에 영향을 받음
	UFUNCTION()
	void UpdateForceBrake();
	// Sleep Angle : Velocity(u, v), Steering Angle을 바탕으로 Sleep Angle을 업데이트
	UFUNCTION()
	void UpdateSleepAngle();
	// Lateral Force(타이어 횡력) : Sleep Angle, 코너링(앞 뒤) 계수를 바탕으로 Lateral Force를 업데이트
	UFUNCTION()
	void UpdateLateralForce();
	// F_drag : 공기 저항 계수를 바탕으로 Fdrag를 업데이트
	UFUNCTION()
	void UpdateDragForce();
	// 바디 종횡 힘을 구함
	UFUNCTION()
	void UpdateBodyForce();
	// F_x : 경사 각도를 바탕으로 F_x를 업데이트 (−mgsin(θ))
	UFUNCTION()
	void UpdateSlopeForce();
	// 병진 운동 방정식을 풀어서 속도를 업데이트
	UFUNCTION()
	void UpdateVelocity();
	// 전역 좌표 갱신
	UFUNCTION()
	void UpdateGlobalCoordinate();
#pragma endregion
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Movement = nullptr;

	// Current Steering Angle : 현재 조향 각도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	float CurrentSteeringAngle = 0.0f;

	// Velocity (x : u : 종방향 속도, y : v : 횡방향 속도, z: 미정)
	UPROPERTY()
	FVector Velocity = FVector::ZeroVector;
	// 차량의 질량
	UPROPERTY()
	float Mass = 1000.0f;
	
	// PlayerInput
	// UPROPERTY()
	FVector2d PlayerInput = FVector2d::ZeroVector;
	
	// Throttle : 가속도 (0.0f ~ 1.0f) : Player의 입력값에 영향을 받음
	UPROPERTY()
	float Throttle = 0.0f;
	UPROPERTY()
	float ThrottleRate = 2.0f;
	// Brake : 감속도 (0.0f ~ 1.0f) : Player의 입력값에 영향을 받음
	UPROPERTY()
	float Brake = 0.0f;
	UPROPERTY()
	float BrakeRate = 0.5f;
	// F_drive : 차량의 종방향 힘 : Throttle에 영향을 받음
	UPROPERTY()
	float F_drive = 0.0f;
	UPROPERTY()
	float F_Max_drive = 300000.0f;
	// F_brake : 차량의 종방향 감속 힘 : Brake에 영향을 받음
	UPROPERTY()
	float F_brake = 0.0f;
	UPROPERTY()
	float F_Max_brake = 150000.0f;
	// Sleep Angle : Velocity(u, v), Steering Angle을 바탕으로 Sleep Angle을 업데이트
	UPROPERTY()
	float SleepAngle = 0.0f;
	UPROPERTY()
	float YawSpeed = 0.0f;
	// 차체 앞/뒤 까지의 길이 (임시 고정)
	UPROPERTY()
	float AL = 100.0f;
	UPROPERTY()
	float BL = 100.0f;
	UPROPERTY()
	float TotalL = AL + BL;
	// Sleep Angle
	UPROPERTY()
	float Alpha_f = 0.0f;
	UPROPERTY()
	float Alpha_r = 0.0f;
	// 앞/뒤 바퀴 코너링 강성
	UPROPERTY()
	float Cf = 5000.0f;
	UPROPERTY()
	float Cr = 5000.0f;
	// Lateral Force(타이어 횡력) : Sleep Angle, 코너링(앞 뒤) 계수를 바탕으로 Lateral Force를 업데이트
	UPROPERTY()
	float Fy_f = 0.0f;
	UPROPERTY()
	float Fy_r = 0.0f;
	// F_drag : 공기 저항 계수를 바탕으로 Fdrag를 업데이트
	UPROPERTY()
	float F_drag = 0.0f; // 우선 상수로 설정
	UPROPERTY()
	float DragCoeff = 0.24f;
	// 바디 종횡 힘을 구함
	UPROPERTY()
	float Fx_total = 0.0f;
	UPROPERTY()
	float Fy_total = 0.0f;
	// Yaw 축 관성 모멘트
	UPROPERTY()
	float I_yaw = 20.0f;
};
