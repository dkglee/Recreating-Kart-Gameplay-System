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
}

bool UKartSuspensionComponent::ProcessSuspension()
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetUpVector() * -SuspensionLength;

	FHitResult HitResult;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(Kart);

	UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitResult, true);
	if (HitResult.bBlockingHit)
	{
		// Offset을 구한 뒤 Offset에 따른 힘을 가해준다.
		float Offset = SuspensionLength - HitResult.Distance;

		// 바퀴의 LinearVelocity를 구한다.
		FVector LinearVelocity = KartBody->GetPhysicsLinearVelocityAtPoint(Start);

		// 바퀴의 UpVector와 LinearVelocity의 내적을 구한다. (위로 향하는 속도를 구함)
		float Velocity = FVector::DotProduct(GetUpVector(), LinearVelocity);

		// 바퀴에 가할 Force를 구함
		float Force = Offset * SpringStrength - Velocity * DamperScale;
		Force = FMath::Clamp(Force, 0.0f, SpringStrength * 50.0f);

		// 바퀴의 UpVector에 Force를 곱해줌
		FVector ForceVector = GetUpVector() * Force;
		KartBody->AddForceAtLocation(ForceVector, Start);
		
		return true;
	}
	return false;
}

