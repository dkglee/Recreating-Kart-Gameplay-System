// Fill out your copyright notice in the Description page of Project Settings.


#include "KartBoosterVFXComponent.h"

#include "Kart.h"
#include "KartBoosterComponent.h"
#include "NiagaraSystem.h"


// Sets default values for this component's properties
UKartBoosterVFXComponent::UKartBoosterVFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	// ...
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NS_BOOSTER
	(TEXT("/Game/Kart/NiagaraEffects/NS_Boost.NS_Boost"));
	if (NS_BOOSTER.Succeeded())
	{
		SetAsset(NS_BOOSTER.Object);
	}

	
}


// Called when the game starts
void UKartBoosterVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartBoosterVFXComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->GetBoosterComponent()->OnBoosterActivated.AddDynamic(this, &UKartBoosterVFXComponent::OnBoosterActivated);
		Kart->GetBoosterComponent()->OnBoosterDeactivated.AddDynamic(this, &UKartBoosterVFXComponent::OnBoosterDeactivated);
	}
}

void UKartBoosterVFXComponent::OnBoosterActivated(float BoosterTime, bool bPowerBoost)
{
	// 서버고 자기 자신이면 부스터 쓰고 Multicast
	if (!Kart->IsLocallyControlled()) return ;

	// 자기 자신은 우선 실행
	ServerRPC_ActivateBoosterVFX_Implementation(BoosterTime, bPowerBoost);
	// 클라이언트는 서버 호출
	if (!Kart->HasAuthority())
	{
		ServerRPC_ActivateBoosterVFX(BoosterTime, bPowerBoost);
	}
	
}

void UKartBoosterVFXComponent::OnBoosterDeactivated()
{
	if (!Kart->IsLocallyControlled()) return ;

	ServerRPC_DeactivateBoosterVFX_Implementation();
	if (!Kart->HasAuthority())
	{
		ServerRPC_DeactivateBoosterVFX();
	}
}

void UKartBoosterVFXComponent::ServerRPC_ActivateBoosterVFX_Implementation(float BoosterTime, bool bPowerBoost)
{
	Deactivate();
	SetFloatParameter(TEXT("BoostTime"), BoosterTime);
	FVector Length = bPowerBoost ? PowerBoostLength : InstantBoostLength;
	SetVectorParameter(TEXT("BoostLength"), Length);
	Activate();
	SetVisibility(true);
	
	MulticastRPC_ActivateBoosterVFX(BoosterTime, bPowerBoost);
}

void UKartBoosterVFXComponent::MulticastRPC_ActivateBoosterVFX_Implementation(float BoosterTime, bool bPowerBoost)
{
	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetFloatParameter(TEXT("BoostTime"), BoosterTime);
		FVector Length = bPowerBoost ? PowerBoostLength : InstantBoostLength;
		SetVectorParameter(TEXT("BoostLength"), Length);
		Activate();
		SetVisibility(true);
	}
}

void UKartBoosterVFXComponent::ServerRPC_DeactivateBoosterVFX_Implementation()
{
	Deactivate();
	SetVisibility(false);

	MulticastRPC_DeactivateBoosterVFX();
}

void UKartBoosterVFXComponent::MulticastRPC_DeactivateBoosterVFX_Implementation()
{
	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetVisibility(false);
	}
}
