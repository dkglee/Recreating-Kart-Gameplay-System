// Fill out your copyright notice in the Description page of Project Settings.


#include "KartPowerBoosterVFXComponent.h"

#include "Kart.h"
#include "KartBoosterComponent.h"
#include "NiagaraSystem.h"


// Sets default values for this component's properties
UKartPowerBoosterVFXComponent::UKartPowerBoosterVFXComponent() : UKartBasicBoosterVFXComponent()
{
	SetRelativeScale3D(FVector(0.03, 0.03, 0.045));
}

// Called when the game starts
void UKartPowerBoosterVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartPowerBoosterVFXComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// UNiagaraSystem* NiagaraAsset = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/RocketThrusterExhaustFX/FX/NS_RocketExhaust_Blue.NS_RocketExhaust_Blue"));
	// if (NiagaraAsset)
	// {
	// 	SetAsset(NiagaraAsset);
	// }

	SetAutoActivate(false);
	Deactivate();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->GetBoosterComponent()->OnBoosterActivated.AddDynamic(this, &UKartPowerBoosterVFXComponent::OnBoosterActivated);
		Kart->GetBoosterComponent()->OnBoosterDeactivated.AddDynamic(this, &UKartPowerBoosterVFXComponent::OnBoosterDeactivated);
	}
}

void UKartPowerBoosterVFXComponent::ServerRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	Super::ServerRPC_ActivateBoosterVFX_Implementation(BoosterTime);

	Deactivate();
	SetFloatParameter(TEXT("LifeTime"), BoosterTime);
	Activate();
	SetVisibility(true);
	
	Super::MulticastRPC_ActivateBoosterVFX(BoosterTime);
}

void UKartPowerBoosterVFXComponent::MulticastRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	Super::MulticastRPC_ActivateBoosterVFX_Implementation(BoosterTime);

	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetFloatParameter(TEXT("LifeTime"), BoosterTime);
		Activate();
		SetVisibility(true);
	}
}

void UKartPowerBoosterVFXComponent::ServerRPC_DeactivateBoosterVFX_Implementation()
{
	Super::ServerRPC_DeactivateBoosterVFX_Implementation();

	Deactivate();
	SetVisibility(false);
	
	Super::MulticastRPC_DeactivateBoosterVFX();
}

void UKartPowerBoosterVFXComponent::MulticastRPC_DeactivateBoosterVFX_Implementation()
{
	Super::MulticastRPC_DeactivateBoosterVFX_Implementation();

	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetVisibility(false);
	}
}

void UKartPowerBoosterVFXComponent::OnBoosterActivated(float BoosterTime)
{
	Super::OnBoosterActivated(BoosterTime);

	if (!Kart->IsLocallyControlled()) return ;

	ServerRPC_ActivateBoosterVFX_Implementation(BoosterTime);
	if (!Kart->HasAuthority())
	{
		ServerRPC_ActivateBoosterVFX(BoosterTime);
	}
}

void UKartPowerBoosterVFXComponent::OnBoosterDeactivated()
{
	Super::OnBoosterDeactivated();

	if (!Kart->IsLocallyControlled()) return ;
	
	ServerRPC_DeactivateBoosterVFX_Implementation();
	if (!Kart->HasAuthority())
	{
		ServerRPC_DeactivateBoosterVFX();
	}
}
