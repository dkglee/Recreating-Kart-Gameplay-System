// Fill out your copyright notice in the Description page of Project Settings.


#include "KartEngineSoundComponent.h"

#include "Kart.h"
#include "KartInfo.h"
#include "KartNetworkSyncComponent.h"
#include "KartSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetStringLibrary.h"


// Sets default values for this component's properties
UKartEngineSoundComponent::UKartEngineSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UKartEngineSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartEngineSoundComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
		KartNetworkSyncComponent = Kart->GetNetworkSyncComponent();
	}
}

void UKartEngineSoundComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


// Called every frame : 모든 클라이언트에게 해당 소리가 들려야 함. 그 말은 각자 알아서 속도 계산하고 소리 내면 된다는 뜻
void UKartEngineSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	PlayKartEngineSound();
}

void UKartEngineSoundComponent::PlayKartEngineSound()
{
	FKartInfo KartInfo = KartNetworkSyncComponent->GetKartInfo();
	FVector LinearVelocity = KartInfo.Velocity;
	FVector ForwardVector = KartBody->GetForwardVector();
	float NormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithVector(LinearVelocity, ForwardVector, Kart->GetMaxSpeed());

	CurrentEngineRPM = MaxEngineRPM * NormalizedSpeed;
	CurrentEngineRPM = FMath::Clamp(CurrentEngineRPM, 0.0f, MaxEngineRPM);

	
}

