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
	// 로컬에서만 상태 변화 감지
	if (Kart->IsLocallyControlled())
	{
		if (bIsSkidActive == bIsSkidding)
			return;

		bIsSkidActive = bIsSkidding;
		ProcessSkidMark_Internal(bIsSkidding);

		// 서버 동기화
		if (!Kart->HasAuthority())
		{
			Server_SetIsSkidActive(bIsSkidding);
		}
	}
}

void UKartSkidMarkComponent::ProcessSkidMark_Internal(bool bIsSkidding)
{
	DrawDebugString(GetWorld(), Kart->GetActorLocation(),
		bIsSkidding ? "Skidding" : "Not Skidding", nullptr, FColor::Red, 0.0f, true);

	if (bIsSkidding)
	{
		Activate(true);
	}
	else
	{
		Deactivate();
	}
}

void UKartSkidMarkComponent::Server_SetIsSkidActive_Implementation(bool bIsSkidding)
{
	bIsSkidActive = bIsSkidding;
	OnRep_bIsSkidActive();
}

bool UKartSkidMarkComponent::Server_SetIsSkidActive_Validate(bool bIsSkidding)
{
	// 필요시 추가 검증 가능
	return true;
}

void UKartSkidMarkComponent::OnRep_bIsSkidActive()
{
	// 리모트 클라이언트에서만 동작
	if (!Kart->IsLocallyControlled())
	{
		ProcessSkidMark_Internal(bIsSkidActive);
	}
}
