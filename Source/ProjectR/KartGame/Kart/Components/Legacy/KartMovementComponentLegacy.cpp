// Fill out your copyright notice in the Description page of Project Settings.

#include "KartMovementComponentLegacy.h"
#include "EnhancedInputComponent.h"
#include "KartSteeringComponentLegacy.h"
#include "Kart.h"

// 단위 변환 상수: 언리얼에서 위치/속도는 보통 cm 단위.
// 우리가 물리 계산은 m 단위로 할 것이므로, cm -> m = 0.01f
static const float CM_TO_M = 0.01f;
static const float M_TO_CM = 100.0f; 

// Sets default values for this component's properties
UKartMovementComponentLegacy::UKartMovementComponentLegacy()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_KARTMOVEMENT
	(TEXT("/Game/Kart/Input/InputAction/IA_KartMovement.IA_KartMovement"));
	if (IA_KARTMOVEMENT.Succeeded())
	{
		IA_Movement = IA_KARTMOVEMENT.Object;
	}
}

// Called when the game starts
void UKartMovementComponentLegacy::BeginPlay()
{
	Super::BeginPlay();

	// Init Steering Delegate
	if (Kart)
	{
		UKartSteeringComponentLegacy* SteeringComp = Kart->FindComponentByClass<UKartSteeringComponentLegacy>();
		if (SteeringComp)
		{
			SteeringComp->OnSteeringDelegate.AddDynamic(this, &UKartMovementComponentLegacy::UpdateSteeringAngle);
		}
	}
}

void UKartMovementComponentLegacy::InitializeComponent()
{
	Super::InitializeComponent();
	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		// 입력 바인딩 델리게이트 등록
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartMovementComponentLegacy::SetupInputBinding);
	}
}

void UKartMovementComponentLegacy::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UKartMovementComponentLegacy::OnMovementInputDetected);
}

// 매 프레임마다 호출되는 이동 입력 함수
void UKartMovementComponentLegacy::OnMovementInputDetected(const FInputActionValue& InputActionValue)
{
	PlayerInput = InputActionValue.Get<FVector2D>();
}

// 스티어링(조향 각도) 업데이트 (조향 컴포넌트에서 Delegate로 호출)
void UKartMovementComponentLegacy::UpdateSteeringAngle(float SteeringAngle)
{
	CurrentSteeringAngle = SteeringAngle;
}

// 매 프레임 이동 처리
void UKartMovementComponentLegacy::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessMovement(DeltaTime);
	
	// 한 프레임마다 입력은 1회 소모된다고 가정하면, 여기서 0으로 리셋
	PlayerInput = FVector2D::ZeroVector;
}

// 실제 물리 계산 로직
void UKartMovementComponentLegacy::ProcessMovement(float DeltaTime)
{
	// 1) Throttle / Brake 갱신
	UpdateThrottle(DeltaTime);
	UpdateBrake(DeltaTime);

	// 2) 구동/브레이크 힘
	UpdateForceDrive();
	UpdateForceBrake();

	// 3) 슬립각 계산
	UpdateSlipAngle();

	// 4) 횡력(코너링 힘)
	UpdateLateralForce();

	// 5) 항력(Drag) 계산 (앞뒤 + 좌우 속도 고려)
	UpdateDragForce();

	// 6) 바디(차체) 좌표계에서 종/횡 합력
	UpdateBodyForce();

	// 7) 경사면(피치/롤) 보정 (간단 버전)
	UpdateSlopeForce();

	// 8) 병진운동 방정식 -> 속도 업데이트
	UpdateVelocity(DeltaTime);

	// 9) 위치/회전(요)을 전역 좌표로 업데이트
	UpdateGlobalCoordinate(DeltaTime);
}

#pragma region Update Functions

void UKartMovementComponentLegacy::UpdateThrottle(float DeltaTime)
{
	// PlayerInput.X를 전진(양수), 후진(음수) 방향으로 가정
	float InputValue = PlayerInput.X;
	
	// 목표 Throttle (양수 입력이면 1, 그렇지 않으면 0)
	float TargetThrottle = (InputValue > 0) ? 1.0f : 0.0f;
	Throttle = FMath::FInterpTo(Throttle, TargetThrottle, DeltaTime, ThrottleRate);
}

void UKartMovementComponentLegacy::UpdateBrake(float DeltaTime)
{
	float InputValue = PlayerInput.X;
	
	// 목표 Brake (음수 입력이면 1, 그렇지 않으면 0)
	float TargetBrake = (InputValue < 0) ? 1.0f : 0.0f;
	Brake = FMath::FInterpTo(Brake, TargetBrake, DeltaTime, BrakeRate);
}

// 가속(드라이브) 힘
void UKartMovementComponentLegacy::UpdateForceDrive()
{
	F_drive = Throttle * F_Max_drive;
}

// 브레이크 힘
void UKartMovementComponentLegacy::UpdateForceBrake()
{
	F_brake = Brake * F_Max_brake;
}

// 슬립각 계산
void UKartMovementComponentLegacy::UpdateSlipAngle()
{
	// Velocity.X, Velocity.Y는 우리가 계속 업데이트하는 값인데,
	// 이 값이 현재 'm/s'라고 가정하겠습니다. (필요하다면 cm/s->m/s 변환해야 함)
	float U = Velocity.X; 
	float V = Velocity.Y; 

	// YawSpeed는 rad/s
	float R = YawSpeed; 

	// (주의) 현실적인 값으로 하려면 차축 간 거리(AL, BL)도 m 단위인지 확인해야 합니다.
	// 현재 AL=1.2, BL=1.2 등으로 수정(약 2.4m 휠베이스) 가정
	if (FMath::Abs(U) < KINDA_SMALL_NUMBER)
	{
		Alpha_f = CurrentSteeringAngle; // 거의 정지상태면 단순화
		Alpha_r = 0.0f;
	}
	else
	{
		Alpha_f = CurrentSteeringAngle - FMath::Atan2((V + AL * R), FMath::Abs(U));
		Alpha_r = -FMath::Atan2((V - BL * R), FMath::Abs(U));
	}

	// 너무 큰 슬립각 방지를 위해(폭주 방지), 예시로 ±15도로 클램핑
	const float MaxSlipAngle = FMath::DegreesToRadians(2.0f);
	Alpha_f = FMath::Clamp(Alpha_f, -MaxSlipAngle, MaxSlipAngle);
	Alpha_r = FMath::Clamp(Alpha_r, -MaxSlipAngle, MaxSlipAngle);

	UE_LOG(LogTemp, Warning, TEXT("Alpha_f : %f, Alpha_r : %f"), Alpha_f, Alpha_r);
}

// 횡력(코너링 강성 모델)
void UKartMovementComponentLegacy::UpdateLateralForce()
{
	// 간단한 선형 모델 Fy = -C * alpha
	// 실제로는 최대 마찰 한도(Fy_max 등)로 포화가 있어야 하지만, 여기서는 단순화
	Fy_f = -Cf * Alpha_f;
	Fy_f = FMath::Clamp(Fy_f, -Fy_Max, Fy_Max);
	Fy_r = -Cr * Alpha_r;
	Fy_r = FMath::Clamp(Fy_r, -Fy_Max, Fy_Max);
}

// 항력(Drag) 계산 - 간단히 속도 제곱비례
void UKartMovementComponentLegacy::UpdateDragForce()
{
	// 속도의 전체 크기를 기준으로 항력을 구하는 방식
	// Fd = DragCoeff * V^2 (방향: 속도 반대방향)
	FVector Vel = FVector(Velocity.X, Velocity.Y, 0.0f);
	float Speed = Vel.Size();  // m/s라고 가정
	
	// 속도가 거의 0이면 드래그도 0
	if (Speed < KINDA_SMALL_NUMBER)
	{
		F_drag = 0.0f;
		return;
	}

	// 항력 크기
	float DragMagnitude = DragCoeff * Speed * Speed;

	// 방향은 속도의 반대 방향
	FVector DragDir = -Vel.GetSafeNormal();
	
	// 여기서는 Fx_total, Fy_total로 분리하기 위해 드래그 벡터 자체를 멤버로 저장
	FVector DragForce = DragDir * DragMagnitude;

	// X, Y만 추출
	DragForceXY = FVector2D(DragForce.X, DragForce.Y);
}

// 종·횡 합력
void UKartMovementComponentLegacy::UpdateBodyForce()
{
	// Fx: (구동력 - 브레이크력) + ...
	// Fy: (앞바퀴 + 뒷바퀴 횡력) + ...
	Fx_total = F_drive - F_brake; 
	Fy_total = Fy_f + Fy_r;

	// 항력도 포함
	Fx_total += DragForceXY.X;
	Fy_total += DragForceXY.Y;
}

// 경사면(피치/롤)으로 인한 추가 종/횡 힘 (간단한 아케이드 식)
void UKartMovementComponentLegacy::UpdateSlopeForce()
{
	float Gravity = 9.81f;

	// Pitch(전후 경사)에 의한 종방향 힘
	float PitchRad = FMath::DegreesToRadians(Kart->GetActorRotation().Pitch);
	float F_slope_pitch = -Mass * Gravity * FMath::Sin(PitchRad);
	Fx_total += F_slope_pitch;

	// Roll(좌우 경사)에 의한 횡방향 힘
	float RollRad = FMath::DegreesToRadians(Kart->GetActorRotation().Roll);
	float F_slope_roll = -Mass * Gravity * FMath::Sin(RollRad);
	Fy_total += F_slope_roll;
}

// 병진 운동 방정식 -> 속도 업데이트
void UKartMovementComponentLegacy::UpdateVelocity(float DeltaTime)
{
	// m(du/dt - r*v) = ΣFx
	// m(dv/dt + r*u) = ΣFy
	// => du/dt = (ΣFx + m*r*v) / m
	// => dv/dt = (ΣFy - m*r*u) / m

	float U = Velocity.X;
	float V = Velocity.Y;
	float r = YawSpeed;

	float DU = (Fx_total + Mass * r * V) / Mass;
	float DV = (Fy_total - Mass * r * U) / Mass;

	float NewU = U + DU * DeltaTime;
	float NewV = V + DV * DeltaTime;

	// 최고 속도 제한(예시)
	const float MaxSpeed = 1000.0f; // m/s로 치면 터무니없이 크므로, 필요에 맞게 조정
	NewU = FMath::Clamp(NewU, -MaxSpeed, MaxSpeed);
	NewV = FMath::Clamp(NewV, -MaxSpeed, MaxSpeed);

	Velocity.X = NewU;
	Velocity.Y = NewV;

	// 요 운동(Iz * dr/dt = a*Fy_f - b*Fy_r ...)
	float TorqueZ = AL * Fy_f - BL * Fy_r - (YawDamping * r);
	float DR = TorqueZ / I_yaw; 

	float NewR = r + DR * DeltaTime;
	YawSpeed = NewR;
}

// 최종 전역 위치/각도 업데이트
void UKartMovementComponentLegacy::UpdateGlobalCoordinate(float DeltaTime)
{
	// 현재 ActorLocation은 cm 단위 => m 단위로 변환
	FVector ActorLocationCM = Kart->GetActorLocation();
	// FVector ActorLocationM = ActorLocationCM * CM_TO_M;
	FVector ActorLocationM = ActorLocationCM;

	// 현재 Yaw는 degrees => radians 변환
	float YawDeg = Kart->GetActorRotation().Yaw;
	float YawRad = FMath::DegreesToRadians(YawDeg);

	// dx/dt = u*cos(yaw) - v*sin(yaw)
	// dy/dt = u*sin(yaw) + v*cos(yaw)
	float U = Velocity.X;
	float V = Velocity.Y;

	float DX = (U * FMath::Cos(YawRad) - V * FMath::Sin(YawRad)) * DeltaTime;
	float DY = (U * FMath::Sin(YawRad) + V * FMath::Cos(YawRad)) * DeltaTime;

	ActorLocationM.X += DX;
	ActorLocationM.Y += DY;

	// 요각 업데이트
	float NewYawRad = YawRad + (YawSpeed * DeltaTime);
	float NewYawDeg = FMath::RadiansToDegrees(NewYawRad);

	// 최종적으로 다시 cm 단위, Deg 단위로 세팅
	// Kart->SetActorLocation(ActorLocationM * M_TO_CM);
	Kart->SetActorLocation(ActorLocationM);
	Kart->SetActorRotation(FRotator(0.0f, NewYawDeg, 0.0f));
}

#pragma endregion

// TODO: 너무 빨리 돌음 YawSpeed 이상함 ㅇㅇ (체크 필요)