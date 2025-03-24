// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSystemLibrary.h"

#include "Kart.h"
#include "Components/BoxComponent.h"

float UKartSystemLibrary::CalculateNormalizedSpeedWithBox(class UBoxComponent* Kart, float MaxSpeed)
{
	float NormalizedSpeed = 0.0f;
	FVector ForwardVector = Kart->GetForwardVector();
	FVector LinearVelocity = Kart->GetPhysicsLinearVelocity();
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);
	NormalizedSpeed = FMath::Abs(KartSpeed) / MaxSpeed;
	return NormalizedSpeed;
}

float UKartSystemLibrary::CalculateNormalizedSpeedWithVector(FVector ForwardVector, FVector LinearVelocity, float MaxSpeed)
{
	float NormalizedSpeed = 0.0f;
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);
	NormalizedSpeed = FMath::Abs(KartSpeed) / MaxSpeed;
	return NormalizedSpeed;
}

float UKartSystemLibrary::CalculateNormalizedSpeedWithKart(class AKart* Kart, float MaxSpeed)
{
	float NormalizedSpeed = 0.0f;
	FVector ForwardVector = Cast<UBoxComponent>(Kart->GetRootComponent())->GetForwardVector();
	FVector LinearVelocity = Cast<UBoxComponent>(Kart->GetRootComponent())->GetPhysicsLinearVelocity();
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);
	NormalizedSpeed = FMath::Abs(KartSpeed) / MaxSpeed;
	return NormalizedSpeed;
}

float UKartSystemLibrary::CalculateNormalizedRightSpeedWithKart(class UBoxComponent* KartBody, float MaxSpeed)
{
	float NormalizedSpeed = 0.0f;
	FVector RightVector = KartBody->GetRightVector();
	FVector LinearVelocity = KartBody->GetPhysicsLinearVelocity();
	float KartSpeed = FVector::DotProduct(RightVector, LinearVelocity);
	NormalizedSpeed = FMath::Abs(KartSpeed) / MaxSpeed;
	return NormalizedSpeed;
}
