// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteractionComponent.h"

#include "FastLogger.h"
#include "Kart.h"


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

	// ...
	
}

void UItemInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
}


// Called every frame
void UItemInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemInteractionComponent::MissileHitInteraction()
{

	UPrimitiveComponent* RootPrimitive = Cast<UPrimitiveComponent>(Kart->GetRootComponent());
	if (RootPrimitive && RootPrimitive->IsSimulatingPhysics())
	{
		FFastLogger::LogConsole(TEXT("Missile Hit!"));
		RootPrimitive->AddImpulse(FVector(0.f,0.f,100000.f));
	}
}

