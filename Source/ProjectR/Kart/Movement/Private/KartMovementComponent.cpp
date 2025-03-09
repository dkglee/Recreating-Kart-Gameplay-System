// Fill out your copyright notice in the Description page of Project Settings.


#include "KartMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "KartSteeringComponent.h"
#include "Kart.h"


// Sets default values for this component's properties
UKartMovementComponent::UKartMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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
void UKartMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// Init Steering Delegate
	Kart->FindComponentByClass<UKartSteeringComponent>()->OnSteeringDelegate.AddDynamic(this, &UKartMovementComponent::UpdateSteeringAngle);
}

void UKartMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartMovementComponent::SetupInputBinding);
	}
}

void UKartMovementComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UKartMovementComponent::OnMovementInputDetected);
}

void UKartMovementComponent::OnMovementInputDetected(const FInputActionValue& InputActionValue)
{
	PlayerInput = InputActionValue.Get<FVector2d>();
	UE_LOG(LogTemp, Log, TEXT("PlayerInput: %s"), *PlayerInput.ToString());
}

void UKartMovementComponent::UpdateSteeringAngle(float SteeringAngle)
{
	CurrentSteeringAngle = SteeringAngle;
}

// Movement Logic
void UKartMovementComponent::ProcessMovement()
{
	// Update Throttle : 플레이어의 입력을 바탕으로 Throttle을 업데이트 (증감)
	UpdateThrottle();
	// Update Brake : 플레이어의 입력을 바탕으로 Brake을 업데이트 (증감)
	UpdateBrake();
	// Update F_drive : Throttle을 바탕으로 Fdrive를 업데이트 (증감)
	UpdateForceDrive();
	// Update F_brake : Throttle을 바탕으로 Fbrake를 업데이트 (증감)
	UpdateForceBrake();
	// Update Sleep Angle : Velocity(u, v), Steering Angle을 바탕으로 Sleep Angle을 업데이트
	UpdateSleepAngle();
	// Update Lateral Force(타이어 횡력) : Sleep Angle, 코너링(앞 뒤) 계수를 바탕으로 Lateral Force를 업데이트
	UpdateLateralForce();
	// Update F_drag : 공기 저항 계수를 바탕으로 Fdrag를 업데이트
	UpdateDragForce();
	// Update F_x, F_y : 바디 좌표계의 종/횡 힘을 구함 (F_drive, F_brake, F_Drag : F_x, Lateral Force : F_y)
	UpdateBodyForce();
	// Update F_x : 경사 각도를 바탕으로 F_x를 업데이트 (−mgsin(θ))
	UpdateSlopeForce();
	// 병진 운동 방정식을 풀어서 속도를 업데이트
	UpdateVelocity();
	// 전역 좌표 Update : 속도를 바탕으로 전역 좌표를 업데이트 (이전에 각도 : Yaw 도 업데이트)
	UpdateGlobalCoordinate();
}

void UKartMovementComponent::UpdateThrottle()
{
	// 입력이 x축 방향으로 들어온다고 가정
	// x축 종방향 Throttle
	float InputValue = PlayerInput.X;
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	float TargetThrottle = (InputValue > 0) ? 1.0f : 0.0f;
	TargetThrottle = FMath::FInterpTo(Throttle, TargetThrottle, DeltaTime, ThrottleRate);
	Throttle = TargetThrottle;
}

void UKartMovementComponent::UpdateBrake()
{
	float InputValue = PlayerInput.X;
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	float TargetBrake = (InputValue < 0) ? 1.0f : 0.0f;
	TargetBrake = FMath::FInterpTo(Brake, TargetBrake, DeltaTime, BrakeRate);
	Brake = TargetBrake;
}

void UKartMovementComponent::UpdateForceDrive()
{
	F_drive = Throttle * F_Max_drive;
	UE_LOG(LogTemp, Warning, TEXT("F_drive: %f"), F_drive);
}

void UKartMovementComponent::UpdateForceBrake()
{
	F_brake = Brake * F_Max_brake;
}

void UKartMovementComponent::UpdateSleepAngle()
{
	// U, V : 바디좌표계 속도, R : 요속도
	// X, Y : 전역좌표계 위치, Yaw : 요각
	float U = Velocity.X;
	float V = Velocity.Y;
	// Yaw 속도
	float R = YawSpeed;

	Alpha_f = 0.0f;
	Alpha_r = 0.0f;

	if (FMath::Abs(U) < KINDA_SMALL_NUMBER)
	{
		// 아주 저속이거나 정지 상태이면, 단순 처리
		Alpha_f = CurrentSteeringAngle;
		Alpha_r = 0.0f;
	}
	else
	{
		Alpha_f = CurrentSteeringAngle - FMath::Atan2((V + AL * R), FMath::Abs(U));
		Alpha_r = -FMath::Atan2((V - BL * R), FMath::Abs(U));
	}
}

void UKartMovementComponent::UpdateLateralForce()
{
	// 타이어 횡력
	Fy_f = -Cf * Alpha_f;
	Fy_r = -Cr * Alpha_r;
	UE_LOG(LogTemp, Warning, TEXT("Fy_f: %f, Fy_r: %f"), Fy_f, Fy_r);
}

void UKartMovementComponent::UpdateDragForce()
{
	// 공기 저항 계수
	F_drag = DragCoeff * (Velocity.X * Velocity.X);
	if (Velocity.X > 0.0) {
		F_drag = F_drag;  // 전진 중이면 음의 방향(감속)
	} else {
		F_drag = -F_drag;  // 후진이면 부호 보정
	}

}

void UKartMovementComponent::UpdateBodyForce()
{
	// 바디좌표계의 힘을 구함 (종/횡)
	Fx_total = F_drive - F_brake - F_drag;
	Fy_total = Fy_f + Fy_r;
}

void UKartMovementComponent::UpdateSlopeForce()
{
	// 경사 각도를 바탕으로 F_x를 업데이트 // 지면의 경사로로 인한 힘으로 구하고 싶으면 월탱처럼 지면의 벡터를 두면 될듯
	// 그리고 힘은 똑같이 줄 수 있음

	// 아래는 우선 아케이드 형으로 가정
	float Gravity = 9.8f;
	
	// 액터의 Pitch 각도 : 종방향 힘에 영향을 줌
	float SlopeAnglePitch = Kart->GetActorRotation().Pitch; // 경사 각도를 어떻게 구할지 고민해보자
	float F_slope_pitch = -Mass * Gravity * FMath::Sin(SlopeAnglePitch);

	// 액터의 Roll 각도 : 횡방향 힘에 영향을 줌
	float SlopeAngleRoll = Kart->GetActorRotation().Roll; // 경사 각도를 어떻게 구할지 고민해보자
	float F_slope_roll = -Mass * Gravity * FMath::Sin(SlopeAngleRoll);

	// 결국엔 이게 종방향에 영향을 주지 않을까?
	Fx_total += F_slope_pitch;
	Fy_total += F_slope_roll;
}

void UKartMovementComponent::UpdateVelocity()
{
	// 5) 병진 운동 방정식
	//    m(du/dt - r*v) = ΣFx
	//    m(dv/dt + r*u) = ΣFy
	// => du/dt = (ΣFx + m*r*v)/m
	//    dv/dt = (ΣFy - m*r*u)/m
	float DU = (Fx_total + Mass * YawSpeed * Velocity.Y) / Mass;
	float DV = (Fy_total - Mass * YawSpeed * Velocity.X) / Mass;

	UE_LOG(LogTemp, Warning, TEXT("Fx_total: %f, Fy_total: %f"), Fx_total, Fy_total);
	UE_LOG(LogTemp, Warning, TEXT("DU: %f, DV: %f"), DU, DV);

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float NewU = Velocity.X + DU * DeltaTime;
	float NewV = Velocity.Y + DV * DeltaTime;

	// 6) 요 운동
	//    Iz * dr/dt = a*Fy_f - b*Fy_r
	float Torque_Z = AL * Fy_f - BL * Fy_r;
	float DR = Torque_Z / I_yaw;
	float NewR = YawSpeed + DR * DeltaTime;

	// update
	Velocity.X = NewU;
	Velocity.Y = NewV;

	UE_LOG(LogTemp, Warning, TEXT("NewU: %f, NewV: %f"), NewU, NewV);
	
	YawSpeed = NewR;
}

void UKartMovementComponent::UpdateGlobalCoordinate()
{
	// 전역 좌표 Update : 속도를 바탕으로 전역 좌표를 업데이트 (이전에 각도 : Yaw 도 업데이트)
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Yaw = Kart->GetActorRotation().Yaw;

	// 7) 전역좌표 갱신
	//    dx/dt = u*cos(yaw) - v*sin(yaw)
	//    dy/dt = u*sin(yaw) + v*cos(yaw)
	float DX = Velocity.X * FMath::Cos(Yaw) - Velocity.Y * FMath::Sin(Yaw);
	float DY = Velocity.X * FMath::Sin(Yaw) + Velocity.Y * FMath::Cos(Yaw);

	float NewX = Kart->GetActorLocation().X + DX * DeltaTime;
	float NewY = Kart->GetActorLocation().Y + DY * DeltaTime;

	// UE_LOG(LogTemp, Warning, TEXT("NewX: %f, NewY: %f"), NewX, NewY);
	
	float NewYaw = Yaw + YawSpeed * DeltaTime;

	Kart->SetActorLocation({NewX, NewY, Kart->GetActorLocation().Z});
	Kart->SetActorRotation({0.0f, NewYaw, 0.0f});
}

// Called every frame
void UKartMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ProcessMovement();
	PlayerInput = FVector2d::ZeroVector;
}

