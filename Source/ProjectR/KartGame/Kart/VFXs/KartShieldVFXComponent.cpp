// Fill out your copyright notice in the Description page of Project Settings.


#include "KartShieldVFXComponent.h"

#include "Kart.h"


// Sets default values for this component's properties
UKartShieldVFXComponent::UKartShieldVFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	SetRelativeLocation(FVector(0,0,0.1));
	SetRelativeScale3D(FVector(0.05));
	// ...
}


// Called when the game starts
void UKartShieldVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	
}

void UKartShieldVFXComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetColorParameter("Color01", FLinearColor(0.1, 0.9, 1));
	SetAutoActivate(false);
	SetForceSolo(true);
	Deactivate();
}




