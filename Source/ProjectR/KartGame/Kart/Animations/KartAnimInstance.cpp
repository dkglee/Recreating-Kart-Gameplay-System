// Fill out your copyright notice in the Description page of Project Settings.


#include "KartAnimInstance.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartSystemLibrary.h"
#include "Components/BoxComponent.h"

UKartAnimInstance::UKartAnimInstance()
{
}

void UKartAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	FFastLogger::LogConsole(TEXT("KartAnimInstance::NativeBeginPlay"));
	Kart = Cast<AKart>(TryGetPawnOwner());
	if (Kart)
	{
		FFastLogger::LogConsole(TEXT("KartAnimInstance::Hello"));
		KartBody = Kart->GetRootBox();
	}
}

void UKartAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Kart) return ;
	
	Accel = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());

	FVector Velocity = KartBody->GetPhysicsLinearVelocity();
	FVector ForwardVector = KartBody->GetForwardVector();
	float Speed = FVector::DotProduct(Velocity, ForwardVector);
	float Sign = FMath::Sign(Speed);
	Rotation += 40.0f * Sign * Accel;
}
