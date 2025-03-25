// Fill out your copyright notice in the Description page of Project Settings.


#include "KartAnimInstance.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartBoosterComponent.h"
#include "KartSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

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

	ProcessRotateWheel();
	ProcessBoosterState();
}

void UKartAnimInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKartAnimInstance, Rotation);
	DOREPLIFETIME(UKartAnimInstance, bBoosterUsing);
}

void UKartAnimInstance::ProcessRotateWheel()
{
	Accel = UKartSystemLibrary::CalculateNormalizedSpeedWithBox(KartBody, Kart->GetMaxSpeed());

	FVector Velocity = KartBody->GetPhysicsLinearVelocity();
	FVector ForwardVector = KartBody->GetForwardVector();
	float Speed = FVector::DotProduct(Velocity, ForwardVector);
	float Sign = FMath::Sign(Speed);
	Rotation += 40.0f * Sign * Accel;
}

void UKartAnimInstance::ProcessBoosterState()
{
	bBoosterUsing = Kart->GetBoosterComponent()->GetbOnBooster();
}
