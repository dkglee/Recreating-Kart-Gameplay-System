// Fill out your copyright notice in the Description page of Project Settings.


#include "KartBoosterComponent.h"

#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartFrictionComponent.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"


// Sets default values for this component's properties
UKartBoosterComponent::UKartBoosterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	// ...
}


// Called when the game starts
void UKartBoosterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartBoosterComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
		AccelerationComponent = Kart->GetAccelerationComponent();

		Kart->GetFrictionComponent()->OnInstantBoost.AddDynamic(this, &UKartBoosterComponent::EnableBoostWindow);
		Kart->GetAccelerationComponent()->OnAccelerationStarted.AddDynamic(this, &UKartBoosterComponent::ProcessInstantBoost);
	}
}

// Called every frame
void UKartBoosterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Kart->IsLocallyControlled()) return;

	ApplyInstantBoost();
}

void UKartBoosterComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UKartBoosterComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		GetWorld()->GetTimerManager().ClearTimer(InstantBoostTimer);
	}

	Super::EndPlay(EndPlayReason);
}


void UKartBoosterComponent::Server_AddBoosterForce_Implementation()
{
	if (Kart->GetAccelerationComponent()->GetTargetAcceleration() == 0)
	{
		Server_ChangebUsingBooster(false);
		OnBoosterDeactivated.Broadcast();
		ElapsedTime = 0.f;
		return;
	}

	ElapsedTime += GetWorld()->GetDeltaSeconds();
	if (ElapsedTime >= BoosterTime)
	{
		Server_ChangebUsingBooster(false);
		OnBoosterDeactivated.Broadcast();
		ElapsedTime = 0.f;
		return;
	}
	
	FVector force = KartBody->GetForwardVector() * KartBody->GetMass() * BoosterForce;
    
    for (int32 i = 0; i < AccelerationComponent->GetWheels().Num(); i++)
    {
    	FVector location = AccelerationComponent->GetWheels()[i]->GetComponentLocation();
    	KartBody->AddForceAtLocation(force, location);
    }
}

void UKartBoosterComponent::ProcessBooster(bool bBoosterUsing)
{
	if (bBoosterUsing)
	{
		Server_AddBoosterForce_Implementation();
	}

	if (bOnBooster != bBoosterUsing)
	{
		bOnBooster = bBoosterUsing;

		if (Kart->HasAuthority())
		{
			ServerRPC_SetbOnBooster_Implementation(bOnBooster);
		}
		else
		{
			ServerRPC_SetbOnBooster(bOnBooster);
		}

		if (bOnBooster)
		{
			OnBoosterActivated.Broadcast(BoosterTime);
		}
	}
}

void UKartBoosterComponent::Server_ChangebUsingBooster_Implementation(bool value)
{
	Kart->SetbUsingBooster(value);
}

void UKartBoosterComponent::ProcessInstantBoost()
{
	if (bInstantBoostEnabled)
	{
		GetWorld()->GetTimerManager().ClearTimer(InstantBoostTimer);
		bInstantBoostEnabled = false;
		bInstantBoostActive = true;
		GetWorld()->GetTimerManager().ClearTimer(InstantBoostActiveTimer);
		TWeakObjectPtr<UKartBoosterComponent> WeakThis = this;
		auto TimerDelegate = FTimerDelegate::CreateLambda([WeakThis]() {
			if (WeakThis.IsValid())
			{
				UKartBoosterComponent* StrongThis = WeakThis.Get();
				StrongThis->bInstantBoostActive = false;
			}
		});
		GetWorld()->GetTimerManager().SetTimer(InstantBoostActiveTimer, TimerDelegate, InstantBoostActiveDuration, false);

		OnInstantBoosterActivated.Broadcast(InstantBoostActiveDuration);
	}
}

void UKartBoosterComponent::EnableBoostWindow()
{
	bInstantBoostEnabled = true;

	GetWorld()->GetTimerManager().ClearTimer(InstantBoostTimer);
	TWeakObjectPtr<UKartBoosterComponent> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(InstantBoostTimer, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			UKartBoosterComponent* BoosterComp = WeakThis.Get();
			if (BoosterComp)
			{
				BoosterComp->bInstantBoostEnabled = false;
			}
		}
	}), InstantBoostDuration, false);
}

void UKartBoosterComponent::ApplyInstantBoost()
{
	if (bInstantBoostActive)
	{
		if (Kart->GetAccelerationComponent()->GetTargetAcceleration() == 0)
		{
			bInstantBoostActive = false;
			OnInstantBoosterDeactivated.Broadcast();
			GetWorld()->GetTimerManager().ClearTimer(InstantBoostActiveTimer);
			return;
		}
		
		FVector force = KartBody->GetForwardVector() * KartBody->GetMass() * BoosterForce * InstantBoostScale;
    
	    for (int32 i = 0; i < AccelerationComponent->GetWheels().Num(); i++)
	    {
	    	FVector location = AccelerationComponent->GetWheels()[i]->GetComponentLocation();
	    	KartBody->AddForceAtLocation(force, location);
	    }
	}
}

void UKartBoosterComponent::ServerRPC_SetbOnBooster_Implementation(bool bInOnBooster)
{
	if (!Kart->IsLocallyControlled())
	{
		bOnBooster = bInOnBooster;
	}

	MulticastRPC_SetbOnBooster(bInOnBooster);
}

void UKartBoosterComponent::MulticastRPC_SetbOnBooster_Implementation(bool bInOnBooster)
{
	if (!Kart->IsLocallyControlled())
	{
		bOnBooster = bInOnBooster;
	}
}
