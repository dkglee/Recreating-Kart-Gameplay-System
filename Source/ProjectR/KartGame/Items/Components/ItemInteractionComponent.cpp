// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteractionComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UItemInteractionComponent::UItemInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

}

// Called when the game starts
void UItemInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemInteractionComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	Kart = Cast<AKart>(GetOwner());
}

void UItemInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsInteraction)
	{
		MissileInteraction_Move(DeltaTime);	
	}

	if (bShieldOn)
	{
		Server_CheckShieldUsingTime();
	}
}

void UItemInteractionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemInteractionComponent, bIsInteraction);
}

void UItemInteractionComponent::MissileHitInteraction()
{
	if (Kart == nullptr)
	{
		FFastLogger::LogConsole(TEXT("InteractionComponent : Kart is nullptr"));
		return;
	}

	if (bShieldOn)
	{
		FFastLogger::LogConsole(TEXT("InteractionComponent : blocked by shield"));
		if (Kart->HasAuthority())
		{
			bShieldOn = false;
		}
		ShieldElapsedTime = 0.f;
		return;
	}

	bIsInteraction = true;
	CurrentType = EInteractionType::Explosion;
	InitialPos = Kart->GetActorLocation();
	InitialQuat = Kart->GetActorQuat();
	Client_ChangePhysics(false);
}

void UItemInteractionComponent::MissileInteraction_Move(float DeltaTime)
{
	if (Kart->HasAuthority() == false) return;

	MissileKnockbackElapsedTime += DeltaTime;
	float alpha = (MissileKnockbackElapsedTime / MissileKnockbackTime);

	// 회전계산
	// ease-out 곡선 적용 (처음엔 빠르게 점점 천천히)
	float easedAlpha = FMath::Sin(alpha * PI * 0.5f);
	float rotationAngle = 360.f * MissileKnockbackRotationNumber * easedAlpha;
		
	// 시작 회전에서 현재까지의 회전 계산
	FQuat rotationQuat = FQuat(FRotator(rotationAngle, 0, 0));
	FQuat resultQuat = rotationQuat * InitialQuat;

	// 위치계산
	float newZ = FMath::Lerp(InitialPos.Z, InitialPos.Z + MissileKnockbackHeight, easedAlpha);
		
	FVector resultPos = FVector(InitialPos.X, InitialPos.Y, newZ);
	
	if (MissileKnockbackElapsedTime >= MissileKnockbackTime)
	{
		bIsInteraction = false;
		CurrentType = EInteractionType::None;
		MissileKnockbackElapsedTime = 0.f;
		Client_ChangePhysics(true);
		return;
	}
	NetMulticast_MissileInteraction_Move(resultQuat, resultPos);
}

void UItemInteractionComponent::NetMulticast_MissileInteraction_Move_Implementation(FQuat resultQuat, FVector resultPos)
{
	Kart->SetActorRotation(resultQuat);
	Kart->SetActorLocation(resultPos);
}

void UItemInteractionComponent::Client_ChangePhysics_Implementation(bool bEnable)
{
	if (Kart)
	{
		if (GetOwnerRole() == ROLE_Authority)
		{
			return;
		}
		Kart->GetRootBox()->SetSimulatePhysics(bEnable);
		Kart->GetAccelerationComponent()->ResetAcceleration();
	}
}

void UItemInteractionComponent::Server_CheckShieldUsingTime_Implementation()
{
	ShieldElapsedTime += GetWorld()->GetDeltaSeconds();
	if (ShieldElapsedTime >= ShieldTime)
	{
		bShieldOn = false;
		ShieldElapsedTime = 0.f;
	}
}
