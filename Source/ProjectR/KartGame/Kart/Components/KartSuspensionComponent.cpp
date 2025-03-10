// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSuspensionComponent.h"
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

	// ...
	bWantsInitializeComponent = true;
}

bool UKartSuspensionComponent::GetLandScapeNormal(FVector& OutNormal, FVector& OutLocation)
{
	OutNormal = LandScapeNormal;
	OutLocation = LandScapeLocation;
	
	return bHitLandScape;
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
	FVector Start = GetComponentLocation();
	FVector End = Start + Kart->GetActorUpVector() * -SuspensionLength;

	FHitResult HitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Kart);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
	if (HitResult.bBlockingHit)
	{
		KartBody->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
		
		float DistanceNormalized = UKismetMathLibrary::NormalizeToRange(HitResult.Distance, 0.0f, SuspensionLength);
		DistanceNormalized = 1 - DistanceNormalized;
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(HitResult.TraceEnd, HitResult.TraceStart);
		
		FVector Force = Direction * DistanceNormalized * ForceScale;
		float ForceLength = Force.Length();

		// Force가 안정화된 값(예: 26464)에 근접하면 고정
		const float StableForce = 26464.0f; // 원하는 Force 크기
		const float Threshold = 100.0f; // 오차 허용 범위

		if (FMath::Abs(ForceLength - StableForce) < Threshold)
		{
			// Force를 StableForce 크기로 고정
			Force = Force.GetSafeNormal() * StableForce;
		}

		if (KartBody)
		{
			KartBody->AddForceAtLocation(Force, Start);
		}
		return true;
	}
	return false;
}

