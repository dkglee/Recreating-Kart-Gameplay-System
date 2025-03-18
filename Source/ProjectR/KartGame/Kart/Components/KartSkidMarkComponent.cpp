// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSkidMarkComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UKartSkidMarkComponent::UKartSkidMarkComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	// ...

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SkidMarkEffectAsset
	(TEXT("/Game/Kart/NiagaraEffects/NS_SkidMark.NS_SkidMark"));
	if (SkidMarkEffectAsset.Succeeded())
	{
		SetAsset(SkidMarkEffectAsset.Object);
	}
	Super::SetAutoActivate(false);
}


// Called when the game starts
void UKartSkidMarkComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	FFastLogger::LogScreen(FColor::Red, TEXT("SkidMark BeginPlay"));

	SetFloatParameter(TEXT("SkidWidth"), 20.0f);
}

void UKartSkidMarkComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
}

void UKartSkidMarkComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKartSkidMarkComponent, bIsSkidActive);
}

void UKartSkidMarkComponent::ProcessSkidMark(bool bIsSkidding)
{
	if (Kart->IsLocallyControlled())
	{
		if (bIsSkidding)
		{
			if (bIsSkidActive) return;
		
			bIsSkidActive = true;
			DrawDebugString(GetWorld(), Kart->GetActorLocation(), "Skidding", nullptr, FColor::Red, 0.0f, true);
			Activate(true);
		}
		else
		{
			if (!bIsSkidActive) return;
		
			bIsSkidActive = false;
			DrawDebugString(GetWorld(), Kart->GetActorLocation(), "Not Skidding", nullptr, FColor::Red, 0.0f, true);
			Deactivate();
		}
	}

	if (!Kart->HasAuthority())
	{
		Server_SetIsSkidActive(bIsSkidding);
	}
}

void UKartSkidMarkComponent::Server_SetIsSkidActive_Implementation(bool bIsSkidding)
{
	bIsSkidActive = bIsSkidding;

	OnRep_bIsSkidActive();
}

void UKartSkidMarkComponent::OnRep_bIsSkidActive()
{
	if (Kart->IsLocallyControlled())
	{
		return ;
	}
	
	if (bIsSkidActive)
	{
		Activate(true);
	}
	else
	{
		Deactivate();
	}
}
