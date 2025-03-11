// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSuspensionComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnitConversion.h"


// Sets default values for this component's properties
UKartSuspensionComponent::UKartSuspensionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
	SetIsReplicatedByDefault(true);
}

// Called when the game starts
void UKartSuspensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartSuspensionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
	}
}


// Called every frame
void UKartSuspensionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	// ProcessSuspension();
}

bool UKartSuspensionComponent::ProcessSuspension()
{
	// TODO: 리펙토링의 여지가 있음
	KartBody->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	
	FVector Start = GetComponentLocation();
	FVector End = Start + Kart->GetActorUpVector() * -SuspensionLength;

	FHitResult HitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Kart);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
	if (HitResult.bBlockingHit)
	{
		float DistanceNormalized = UKismetMathLibrary::NormalizeToRange(HitResult.Distance, 0.0f, SuspensionLength);
		DistanceNormalized = 1 - DistanceNormalized;
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(HitResult.TraceEnd, HitResult.TraceStart);

		// Damper 안정성 조정
		float Damper = FVector::DotProduct(Kart->GetVelocity(), GetUpVector()) * -DamperScale;
		float ResultForceScale = Damper + ForceScale;
		
		FVector Force = Direction * DistanceNormalized * ResultForceScale;

		if (KartBody)
		{
			KartBody->AddForceAtLocation(Force, Start);
		}
		return true;
	}
	return false;
}

