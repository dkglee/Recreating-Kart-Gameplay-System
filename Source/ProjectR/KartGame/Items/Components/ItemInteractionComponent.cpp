// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteractionComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UItemInteractionComponent::UItemInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

}


// Called when the game starts
void UItemInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	Kart = Cast<AKart>(GetOwner());

}

void UItemInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UItemInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInteraction)
	{
		if (CurrentType == EInteractionType::Explosion)
		{
			MissileKnockbackElapsedTime += DeltaTime;
			float alpha = (MissileKnockbackElapsedTime / MissileKnockbackTime);

			// 회전계산
			// ease-out 곡선 적용 (처음엔 빠르게 점점 천천히)
			float easedAlpha = FMath::Sin(alpha * PI * 0.5f);
			float rotationAngle = 360.f * MissileKnockbackRotationNumber * easedAlpha;
			
			// 시작 회전에서 현재까지의 회전 계산
			FQuat rotationQuat = FQuat(FRotator(rotationAngle, 0, 0));
			FQuat resultQuat = rotationQuat * InitialQuat;
			Kart->SetActorRotation(resultQuat);

			// 위치계산
			float newZ = FMath::Lerp(InitialPos.Z, InitialPos.Z + MissileKnockbackHeight, easedAlpha);
			
			FVector newPos = FVector(InitialPos.X, InitialPos.Y, newZ);
			Kart->SetActorLocation(newPos);
	    
			if (MissileKnockbackElapsedTime >= MissileKnockbackTime)
			{
				bIsInteraction = false;
				CurrentType = EInteractionType::None;
				Kart->GetRootBox()->SetSimulatePhysics(true);
				MissileKnockbackElapsedTime = 0.f;
			}
		}
	}
}

void UItemInteractionComponent::MissileHitInteraction()
{
	if (Kart == nullptr)
	{
		FFastLogger::LogConsole(TEXT("TargetKart is nullptr"));
		return;
	}
	bIsInteraction = true;
	CurrentType = EInteractionType::Explosion;
	Kart->GetRootBox()->SetSimulatePhysics(false);
	InitialPos = Kart->GetActorLocation();
	InitialQuat = Kart->GetActorQuat();
}
