// Fill out your copyright notice in the Description page of Project Settings.
#include "KartSteeringComponent.h"

#include "EnhancedInputComponent.h"
#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "Math/BigInt.h"

// Sets default values for this component's properties
UKartSteeringComponent::UKartSteeringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	// SetIsReplicated(true);
	// ...
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_STEERING
	(TEXT("/Game/Kart/Input/InputAction/IA_KartSteering.IA_KartSteering"));
	if (IA_STEERING.Succeeded())
	{
		IA_Steering = IA_STEERING.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> C_STEERINGCURVE
	(TEXT("/Game/Kart/Curves/SteeringCurve.SteeringCurve"));
	if (C_STEERINGCURVE.Succeeded())
	{
		SteeringCurve = C_STEERINGCURVE.Object;
	}
}


// Called when the game starts
void UKartSteeringComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartSteeringComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartSteeringComponent::SetupInputBinding);
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
	}
}

void UKartSteeringComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Steering, ETriggerEvent::Triggered, this, &UKartSteeringComponent::OnSteeringInputDetected);
	PlayerInputComponent->BindAction(IA_Steering, ETriggerEvent::Completed, this, &UKartSteeringComponent::OnSteeringInputDetected);
}

// Called every frame
void UKartSteeringComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UKartSteeringComponent::ProcessSteeringAndTorque()
{
	if (Kart->HasAuthority())
	{
		ApplySteeringToKart_Implementation(TargetSteering);
		ApplyTorqueToKartV2_Implementation(SteeringIntensity);
	}
	else if (Kart->GetLocalRole() == ROLE_AutonomousProxy)
	{
		// 로컬 및 서버 둘 다 실행
		ApplySteeringToKart_Implementation(TargetSteering);
		ApplySteeringToKart(TargetSteering);
		ApplyTorqueToKartV2(SteeringIntensity);
	}
}

void UKartSteeringComponent::ApplyTorqueToKart_Implementation(float InAccelerationIntensity, float InSteeringIntensity)
{
	// 1. 카트의 로컬 축 벡터 구하기
	// FVector RightVector = KartBody->GetRightVector();     // Roll 축 (X)
	FVector UpVector = KartBody->GetUpVector();           // Pitch 축 (Y)
	FVector ForwardVector = KartBody->GetForwardVector(); // Yaw 축 (Z) - 기울어진 상태 고려됨

	// 2. 현재 속도 방향 구하기
	FVector Velocity = KartBody->GetComponentVelocity();
	ForwardVector.Normalize();
	Velocity.Normalize();

	// 3. 전진/후진 여부 확인
	float ForwardIntensity = FVector::DotProduct(ForwardVector, Velocity);
	float DirectionSign = FMath::Sign(ForwardIntensity); 

	// 4. Steering Torque를 로컬 Yaw 축 기준으로 적용
	float YawTorqueValue = InSteeringIntensity * SteerPower * DirectionSign;

	// 5. 로컬 Yaw 축을 기준으로 Torque 벡터 생성
	FVector LocalYawAxis = UpVector; // 카트의 현재 Up 벡터가 기울어진 상태의 Yaw 축
	FVector Torque = LocalYawAxis * YawTorqueValue;

	// 6. 속도가 충분할 때만 토크 적용
	if (FMath::Abs(InAccelerationIntensity) > 0.05f) {
		KartBody->AddTorqueInRadians(Torque);
	}
}

void UKartSteeringComponent::OnSteeringInputDetected(const FInputActionValue& InputActionValue)
{
	TargetSteering = InputActionValue.Get<float>();
	FFastLogger::LogConsole(TEXT("Target Steering Value: %f"), TargetSteering);
}

// 해당 함수는 앞바퀴를 회전하는 용도로 사용될 거임
void UKartSteeringComponent::ApplySteeringToKart_Implementation(float InTargetSteering)
{
	// 카트 바디의 앞바퀴들을 가져옴
	UKartSuspensionComponent* LF_Wheel = Kart->GetLF_Wheel();
	UKartSuspensionComponent* RF_Wheel = Kart->GetRF_Wheel();

	TArray<UKartSuspensionComponent*> Wheels = {LF_Wheel, RF_Wheel};

	SteerRate = FMath::Abs(InTargetSteering) ? SteerRate : SteerRate * 2.0f;

	SteeringIntensity = FMath::FInterpTo(SteeringIntensity, InTargetSteering, GetWorld()->GetDeltaSeconds(), SteerRate);

	// Rotate the wheels
	for (UKartSuspensionComponent* Wheel : Wheels)
	{
		float Alpha = (SteeringIntensity + 1.0f) / 2.0f;
		FRotator NewRotation = FMath::Lerp(FRotator{0, MaxRotation, 0}, FRotator{0, -MaxRotation, 0}, Alpha);
		Wheel->SetRelativeRotation(NewRotation);
	}
}

// 해당 함수는 실질적으로 Kart Body에 Torque를 가할 때 사용될 거임
void UKartSteeringComponent::ApplyTorqueToKartV2_Implementation(float InSteering)
{
	float InNormalizedSpeed = Kart->GetNormalizedSpeed();
	float SteeringPower = SteeringCurve->GetFloatValue(InNormalizedSpeed);

	FVector ForwardVector = KartBody->GetForwardVector();
	FVector LinearVelocity = KartBody->GetPhysicsLinearVelocity();
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);
	float KartSign = FMath::Sign(KartSpeed);
	
	float TurnValue = InSteering * SteeringPower * TurnScaling * KartSign;
	// TODO: 땅에 닿았을 때만 할 수 있도록 조건을 추가할지 말지 고민해야 함. (추후에)
	// TurnValue = bGrounded ? TurnValue : 0.0f;
	
	// Torque Vector 생성
	FVector KartUpVector = KartBody->GetUpVector();
	FVector Torque = KartUpVector * TurnValue;

	// KartBody에 Torque 적용
	KartBody->AddTorqueInDegrees(Torque, NAME_None, true);
}
