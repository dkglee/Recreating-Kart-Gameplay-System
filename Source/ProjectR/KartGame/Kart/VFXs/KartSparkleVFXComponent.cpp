// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSparkleVFXComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartFrictionComponent.h"


// Sets default values for this component's properties
UKartSparkleVFXComponent::UKartSparkleVFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	Super::SetAutoActivate(false);
	// ...

	SetRelativeLocation({10, 0, -25});
	SetRelativeRotation({-90, 0, 0});
	SetRelativeScale3D({0.1, 0.1, 0.2});
}


// Called when the game starts
void UKartSparkleVFXComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UKartSparkleVFXComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKartSparkleVFXComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetForceSolo(true);
	
	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->GetFrictionComponent()->OnDriftStarted.AddDynamic(this, &UKartSparkleVFXComponent::ActivateSparkleVFX);
		Kart->GetFrictionComponent()->OnDriftEnded.AddDynamic(this, &UKartSparkleVFXComponent::DeactivateSparkleVFX);
	}
}

void UKartSparkleVFXComponent::ActivateSparkleVFX()
{
	if (!Kart->IsLocallyControlled()) return ;

	ServerRPC_ActivateSparkleVFX_Implementation();
	if (!Kart->HasAuthority())
	{
		ServerRPC_ActivateSparkleVFX();
	}
}

void UKartSparkleVFXComponent::DeactivateSparkleVFX()
{
	if (!Kart->IsLocallyControlled()) return ;

	ServerRPC_DeactivateSparkleVFX_Implementation();
	if (!Kart->HasAuthority())
	{
		ServerRPC_DeactivateSparkleVFX();
	}
}

void UKartSparkleVFXComponent::ServerRPC_ActivateSparkleVFX_Implementation()
{
	Deactivate();
	Activate();
	SetVisibility(true);

	MulticastRPC_ActivateSparkleVFX_Implementation();
}

void UKartSparkleVFXComponent::MulticastRPC_ActivateSparkleVFX_Implementation()
{
	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		Activate();
		SetVisibility(true);
	}
}

void UKartSparkleVFXComponent::ServerRPC_DeactivateSparkleVFX_Implementation()
{
	Deactivate();
	SetVisibility(false);

	MulticastRPC_DeactivateSparkleVFX();
}

void UKartSparkleVFXComponent::MulticastRPC_DeactivateSparkleVFX_Implementation()
{
	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetVisibility(false);
	}
}
