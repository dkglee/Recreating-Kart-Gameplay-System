// Fill out your copyright notice in the Description page of Project Settings.


#include "Kart/Suspension/Public/KartSuspensionComponent.h"

#include "AssetSelection.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values for this component's properties
UKartSuspensionComponent::UKartSuspensionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bWantsInitializeComponent = true;
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
		KartBody->SetLinearDamping(5.0f);
		KartBody->SetMassOverrideInKg(NAME_None, 150.0f, true);
	}
}


// Called every frame
void UKartSuspensionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ProcessSuspension();
}

void UKartSuspensionComponent::ProcessSuspension()
{
	FVector Start = GetComponentLocation();
	FVector End = Start + FVector({0.0f, 0.0f, -60.0f});

	FHitResult HitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Kart);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
	if (HitResult.bBlockingHit)
	{
		float DistanceNormalized = UKismetMathLibrary::NormalizeToRange(HitResult.Distance, 0.0f, 60.0f);
		DistanceNormalized = 1 - DistanceNormalized;
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(HitResult.TraceEnd, HitResult.TraceStart);

		FVector Force = Direction * DistanceNormalized * ForceScale;
		if (KartBody)
		{
			UE_LOG(LogTemp, Warning, TEXT("Force: %s, this: %p"), *Force.ToString(), this);
			KartBody->AddForceAtLocation(Force, Start);
		}
	}
}

