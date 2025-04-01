// Fill out your copyright notice in the Description page of Project Settings.


#include "KartInstantBoostVFXComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartBoosterComponent.h"
#include "NiagaraSystem.h"


// Sets default values for this component's properties
UKartInstantBoostVFXComponent::UKartInstantBoostVFXComponent() : UKartBasicBoosterVFXComponent()
{
	SetRelativeScale3D(FVector(0.03, 0.03, 0.045));
}

// Called when the game starts
void UKartInstantBoostVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartInstantBoostVFXComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// UNiagaraSystem* NiagaraAsset = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/RocketThrusterExhaustFX/FX/NS_RocketExhaust_Afterburn.NS_RocketExhaust_Afterburn"));
	// if (NiagaraAsset)
	// {
	// 	SetAsset(NiagaraAsset);
	// }
	
	SetAutoActivate(false);
	Deactivate();
	
	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->GetBoosterComponent()->OnInstantBoosterActivated.AddDynamic(this, &UKartInstantBoostVFXComponent::OnBoosterActivated);
		Kart->GetBoosterComponent()->OnInstantBoosterDeactivated.AddDynamic(this, &UKartInstantBoostVFXComponent::OnBoosterDeactivated);
	}
}

// Called every frame
void UKartInstantBoostVFXComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKartInstantBoostVFXComponent::ServerRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	Super::ServerRPC_ActivateBoosterVFX_Implementation(BoosterTime);

	Deactivate();
	SetFloatParameter(TEXT("LifeTime"), BoosterTime);
	Activate();
	SetVisibility(true);
	TWeakObjectPtr<UKartInstantBoostVFXComponent> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			UKartInstantBoostVFXComponent* StrongThis = WeakThis.Get();
			StrongThis->Deactivate();
			StrongThis->SetVisibility(false);
		}
	}), BoosterTime, false);
	
	Super::MulticastRPC_ActivateBoosterVFX(BoosterTime);
}

void UKartInstantBoostVFXComponent::MulticastRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	Super::MulticastRPC_ActivateBoosterVFX_Implementation(BoosterTime);

	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetFloatParameter(TEXT("LifeTime"), BoosterTime);
		Activate();
		SetVisibility(true);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UKartInstantBoostVFXComponent::MulticastRPC_DeactivateBoosterVFX_Implementation, BoosterTime, false);
	}
}

void UKartInstantBoostVFXComponent::ServerRPC_DeactivateBoosterVFX_Implementation()
{
	Super::ServerRPC_DeactivateBoosterVFX_Implementation();

	Deactivate();
	SetVisibility(false);
	
	Super::MulticastRPC_DeactivateBoosterVFX();
}

void UKartInstantBoostVFXComponent::MulticastRPC_DeactivateBoosterVFX_Implementation()
{
	Super::MulticastRPC_DeactivateBoosterVFX_Implementation();

	if (!Kart->IsLocallyControlled())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		Deactivate();
		SetVisibility(false);
	}
}

void UKartInstantBoostVFXComponent::OnBoosterActivated(float BoosterTime)
{
	Super::OnBoosterActivated(BoosterTime);

	// 서버고 자기 자신이면 부스터 쓰고 Multicast
	if (!Kart->IsLocallyControlled()) return ;
	
	// 자기 자신은 우선 실행
	ServerRPC_ActivateBoosterVFX_Implementation(BoosterTime);
	// 클라이언트는 서버 호출
	if (!Kart->HasAuthority())
	{
		ServerRPC_ActivateBoosterVFX(BoosterTime);
	}
}

void UKartInstantBoostVFXComponent::OnBoosterDeactivated()
{
	Super::OnBoosterDeactivated();

	if (!Kart->IsLocallyControlled()) return ;
	
	ServerRPC_DeactivateBoosterVFX_Implementation();
	if (!Kart->HasAuthority())
	{
		ServerRPC_DeactivateBoosterVFX();
	}
}
