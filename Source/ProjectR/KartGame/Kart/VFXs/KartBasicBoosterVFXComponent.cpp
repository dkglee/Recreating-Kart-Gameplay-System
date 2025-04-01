// Fill out your copyright notice in the Description page of Project Settings.


#include "KartBasicBoosterVFXComponent.h"

#include "Kart.h"
#include "KartBoosterComponent.h"
#include "NiagaraSystem.h"

// Sets default values for this component's properties
UKartBasicBoosterVFXComponent::UKartBasicBoosterVFXComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	
	Super::SetAutoActivate(false);
}


// Called when the game starts
void UKartBasicBoosterVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetForceSolo(true);
}

void UKartBasicBoosterVFXComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UKartBasicBoosterVFXComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}

void UKartBasicBoosterVFXComponent::OnBoosterActivated(float BoosterTimer)
{
	// // 서버고 자기 자신이면 부스터 쓰고 Multicast
	// if (!Kart->IsLocallyControlled()) return ;
	//
	// // 자기 자신은 우선 실행
	// ServerRPC_ActivateBoosterVFX_Implementation(BoosterTime, bPowerBoost);
	// // 클라이언트는 서버 호출
	// if (!Kart->HasAuthority())
	// {
	// 	ServerRPC_ActivateBoosterVFX(BoosterTime, bPowerBoost);
	// }
	
}

void UKartBasicBoosterVFXComponent::OnBoosterDeactivated()
{
	// if (!Kart->IsLocallyControlled()) return ;
	//
	// ServerRPC_DeactivateBoosterVFX_Implementation();
	// if (!Kart->HasAuthority())
	// {
	// 	ServerRPC_DeactivateBoosterVFX();
	// }
}

void UKartBasicBoosterVFXComponent::ServerRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	// Deactivate();
	// SetFloatParameter(TEXT("BoostTime"), BoosterTime);
	// FVector Length = bPowerBoost ? PowerBoostLength : InstantBoostLength;
	// SetVectorParameter(TEXT("BoostLength"), Length);
	// Activate();
	// SetVisibility(true);
	//
	// MulticastRPC_ActivateBoosterVFX(BoosterTime, bPowerBoost);
}

void UKartBasicBoosterVFXComponent::MulticastRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	// if (!Kart->IsLocallyControlled())
	// {
	// 	Deactivate();
	// 	SetFloatParameter(TEXT("BoostTime"), BoosterTime);
	// 	FVector Length = bPowerBoost ? PowerBoostLength : InstantBoostLength;
	// 	SetVectorParameter(TEXT("BoostLength"), Length);
	// 	Activate();
	// 	SetVisibility(true);
	// }
}

void UKartBasicBoosterVFXComponent::ServerRPC_DeactivateBoosterVFX_Implementation()
{
	// Deactivate();
	// SetVisibility(false);
	//
	// MulticastRPC_DeactivateBoosterVFX();
}

void UKartBasicBoosterVFXComponent::MulticastRPC_DeactivateBoosterVFX_Implementation()
{
	// if (!Kart->IsLocallyControlled())
	// {
	// 	Deactivate();
	// 	SetVisibility(false);
	// }
}