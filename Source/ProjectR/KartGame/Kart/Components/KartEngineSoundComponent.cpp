// Fill out your copyright notice in the Description page of Project Settings.


#include "KartEngineSoundComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartInfo.h"
#include "KartNetworkSyncComponent.h"
#include "KartSystemLibrary.h"
#include "AssetTypeActions/AssetDefinition_SoundBase.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/KismetStringLibrary.h"

// Sets default values for this component's properties
UKartEngineSoundComponent::UKartEngineSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(false);
	// ...

	static ConstructorHelpers::FObjectFinder<USoundBase> MSS_ENGINE
	(TEXT("/Game/Kart/Sounds/MSS_Engine.MSS_Engine"));
	if (MSS_ENGINE.Succeeded())
	{
		EngineSoundSource = MSS_ENGINE.Object;
		SetSound(EngineSoundSource);
	}

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> SA_ENGINE
	(TEXT("/Game/Kart/Sounds/SA_Engine.SA_Engine"));
	if (SA_ENGINE.Succeeded())
	{
		EngineSoundAttenuation = SA_ENGINE.Object;
		SetAttenuationSettings(EngineSoundAttenuation);
	}
}


// Called when the game starts
void UKartEngineSoundComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (Kart->IsLocallyControlled())
	{
		DrawDebugString(GetWorld(), KartBody->GetComponentLocation(), TEXT("Engine Sound Component Begin Play"), nullptr, FColor::Red, 0.0f);
		Play();
	}
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

void UKartEngineSoundComponent::PlayKartEngineSound()
{
	FKartInfo KartInfo = KartNetworkSyncComponent->GetKartInfo();
	FVector LinearVelocity = KartInfo.Velocity;
	FVector ForwardVector = KartBody->GetForwardVector();
	float NormalizedSpeed = UKartSystemLibrary::CalculateNormalizedSpeedWithVector(LinearVelocity, ForwardVector, Kart->GetMaxSpeed());
	
	CurrentEngineRPM = MaxEngineRPM * NormalizedSpeed;
	CurrentEngineRPM = FMath::Clamp(CurrentEngineRPM, 0.0f, MaxEngineRPM);
	
	EnginePitch = 1.0f * NormalizedSpeed;
	EnginePitch = FMath::Clamp(EnginePitch, MinEnginePitch, MaxEnginePitch);
	
	if (NormalizedSpeed > 1.10f)
	{
		EnginePitchShift = 2.0f;
	}
	else
	{
		EnginePitchShift = MaxEnginePitchShift * (1.0f - NormalizedSpeed);
	}
	
	SetPitchMultiplier(EnginePitch);
	SetVolumeMultiplier(EngineVolume);
	SetFloatParameter(TEXT("PitchShift"), EnginePitchShift);
	
	// NormalizedSpeed = NormalizedSpeed > 0.3f ? 1.0f : NormalizedSpeed;
	NormalizedSpeed *= 1.5f;
	NormalizedSpeed = FMath::Clamp(NormalizedSpeed, 0.0f, 1.0f);
	SetFloatParameter(TEXT("Acceleration"), NormalizedSpeed);
	
	FString DebugString = FString::Printf(TEXT("NormalizedSpeed: %f\r\nEnginePitch: %f\r\nEnginePitchShift: %f"), NormalizedSpeed, EnginePitch, EnginePitchShift);
	DrawDebugString(GetWorld(), KartBody->GetComponentLocation(), DebugString, nullptr, FColor::Red, 0.0f);
}

