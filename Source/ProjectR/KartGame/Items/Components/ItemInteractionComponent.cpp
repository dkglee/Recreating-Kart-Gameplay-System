// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInteractionComponent.h"

#include "FastLogger.h"
#include "Kart.h"
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
		MissileInteractionMove(DeltaTime);	
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

	if (Kart->HasAuthority())
	{
		bIsInteraction = true;
		CurrentType = EInteractionType::Explosion;
		InitialPos = Kart->GetActorLocation();
		InitialQuat = Kart->GetActorQuat();
	}
}

void UItemInteractionComponent::MissileInteractionMove(float DeltaTime)
{
	if (Kart->HasAuthority())
	{
		Server_MissileInteractionMove(DeltaTime);
	}
}

void UItemInteractionComponent::Server_MissileInteractionMove_Implementation(float DeltaTime)
{
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

	NetMulticast_MissileInteractionMove(resultQuat, resultPos);
    
	if (MissileKnockbackElapsedTime >= MissileKnockbackTime)
	{
		bIsInteraction = false;
		CurrentType = EInteractionType::None;
		MissileKnockbackElapsedTime = 0.f;
	}
}

void UItemInteractionComponent::NetMulticast_MissileInteractionMove_Implementation(FQuat resultQuat, FVector resultPos)
{
	Kart->SetActorRotation(resultQuat);
	Kart->SetActorLocation(resultPos);
}
