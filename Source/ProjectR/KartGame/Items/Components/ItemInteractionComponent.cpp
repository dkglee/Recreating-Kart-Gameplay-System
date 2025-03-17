// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteractionComponent.h"

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

	// ...
}


// Called when the game starts
void UItemInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	Kart = Cast<AKart>(GetOwner());

	// ...
}

void UItemInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame
void UItemInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInteraction)
	{
		KnockbackElapsedTime += DeltaTime;
		float alpha = (KnockbackElapsedTime / KnockbackTime);
		float easedAlpha = FMath::Sin(alpha * PI * 0.5f);
		float rotationAngle = 360.f * 2.f * easedAlpha;
    
		// 시작 회전에서 현재까지의 회전 계산
		FQuat rotationQuat = FQuat(FRotator(rotationAngle, 0, 0));
		FQuat resultQuat = rotationQuat * InitialQuat;
		Kart->SetActorRotation(resultQuat);
    
		if (KnockbackElapsedTime >= KnockbackTime)
		{
			bIsInteraction = false;
			Kart->GetRootBox()->SetSimulatePhysics(true);
			KnockbackElapsedTime = 0.f;
		}
	}
}

void UItemInteractionComponent::MissileHitInteraction()
{
	if (Kart == nullptr) return;
	bIsInteraction = true;
	Kart->GetRootBox()->SetSimulatePhysics(false);
	FVector newPos = Kart->GetActorLocation() + FVector(0,0,500.f);
	Kart->SetActorLocation(newPos);
	InitialQuat = Kart->GetActorQuat();
}
