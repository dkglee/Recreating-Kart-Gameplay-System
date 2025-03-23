// Fill out your copyright notice in the Description page of Project Settings.


#include "KartBoosterComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UKartBoosterComponent::UKartBoosterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UKartBoosterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartBoosterComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
		AccelerationComponent = Kart->GetAccelerationComponent();
	}
}

// Called every frame
void UKartBoosterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UKartBoosterComponent::ProcessBooster(bool bBoosterUsing)
{
	if (bBoosterUsing)
	{
		Server_AddBoosterForce_Implementation();
	}
}

void UKartBoosterComponent::Server_AddBoosterForce_Implementation()
{
	FVector force = KartBody->GetForwardVector() * KartBody->GetMass() * BoosterForce;

	for (int32 i = 0; i < AccelerationComponent->GetWheels().Num(); i++)
	{
		FVector location = AccelerationComponent->GetWheels()[i]->GetComponentLocation();
		KartBody->AddForceAtLocation(force, location);
	}
}
