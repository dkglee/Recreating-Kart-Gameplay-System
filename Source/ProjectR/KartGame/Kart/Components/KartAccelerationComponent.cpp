// Fill out your copyright notice in the Description page of Project Settings.


#include "KartAccelerationComponent.h"


// Sets default values for this component's properties
UKartAccelerationComponent::UKartAccelerationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bWantsInitializeComponent = true;
}


// Called when the game starts
void UKartAccelerationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartAccelerationComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame
void UKartAccelerationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

