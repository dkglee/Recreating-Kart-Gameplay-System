// Fill out your copyright notice in the Description page of Project Settings.


#include "KartAirBoostVFXComponent.h"

#include "Kart.h"
#include "KartBoosterComponent.h"


// Sets default values for this component's properties
UKartAirBoostVFXComponent::UKartAirBoostVFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	SetRelativeLocation(FVector(-5.743609, 0, 0));
	SetRelativeRotation(FRotator(-90, 0, 0));
	SetRelativeScale3D(FVector(0.01, 0.01, 0.01));
}


// Called when the game starts
void UKartAirBoostVFXComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKartAirBoostVFXComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}

void UKartAirBoostVFXComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	SetAutoActivate(false);
	Deactivate();
	
	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->GetBoosterComponent()->OnInstantBoosterActivated.AddDynamic(this, &UKartAirBoostVFXComponent::OnBoosterActivated);
		Kart->GetBoosterComponent()->OnInstantBoosterDeactivated.AddDynamic(this, &UKartAirBoostVFXComponent::OnBoosterDeactivated);
		Kart->GetBoosterComponent()->OnBoosterActivated.AddDynamic(this, &UKartAirBoostVFXComponent::OnBoosterActivated);
		Kart->GetBoosterComponent()->OnBoosterDeactivated.AddDynamic(this, &UKartAirBoostVFXComponent::OnBoosterDeactivated);
	}
}

void UKartAirBoostVFXComponent::ServerRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	Super::ServerRPC_ActivateBoosterVFX_Implementation(BoosterTime);

	Deactivate();
	SetFloatParameter(TEXT("LifeTimeBoost"), BoosterTime);
	Activate();
	SetVisibility(true);
	
	Super::MulticastRPC_ActivateBoosterVFX(BoosterTime);
}

void UKartAirBoostVFXComponent::MulticastRPC_ActivateBoosterVFX_Implementation(float BoosterTime)
{
	Super::MulticastRPC_ActivateBoosterVFX_Implementation(BoosterTime);

	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetFloatParameter(TEXT("LifeTimeBoost"), BoosterTime);
		Activate();
		SetVisibility(true);
	}
}

void UKartAirBoostVFXComponent::ServerRPC_DeactivateBoosterVFX_Implementation()
{
	Super::ServerRPC_DeactivateBoosterVFX_Implementation();

	Deactivate();
	SetVisibility(false);
	
	Super::MulticastRPC_DeactivateBoosterVFX();
}

void UKartAirBoostVFXComponent::MulticastRPC_DeactivateBoosterVFX_Implementation()
{
	Super::MulticastRPC_DeactivateBoosterVFX_Implementation();

	if (!Kart->IsLocallyControlled())
	{
		Deactivate();
		SetVisibility(false);
	}
}

void UKartAirBoostVFXComponent::OnBoosterActivated(float BoosterTime)
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

void UKartAirBoostVFXComponent::OnBoosterDeactivated()
{
	Super::OnBoosterDeactivated();
	
	if (!Kart->IsLocallyControlled()) return ;
	
	ServerRPC_DeactivateBoosterVFX_Implementation();
	if (!Kart->HasAuthority())
	{
		ServerRPC_DeactivateBoosterVFX();
	}
}
