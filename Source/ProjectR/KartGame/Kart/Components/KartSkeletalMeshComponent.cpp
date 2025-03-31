// Fill out your copyright notice in the Description page of Project Settings.


#include "KartSkeletalMeshComponent.h"

#include "Kart.h"
#include "KartAnimInstance.h"

UKartSkeletalMeshComponent::UKartSkeletalMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	Super::SetComponentTickEnabled(false);
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKM_Kart
	(TEXT("/Game/Kart/Assets/r_01541.r_01541"));
	if (SKM_Kart.Succeeded())
	{
		Super::SetSkeletalMesh(SKM_Kart.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_KartAnimClass
	(TEXT("/Game/Kart/Animations/ABP_KartAnimInstance.ABP_KartAnimInstance_C"));
	if (ABP_KartAnimClass.Succeeded())
	{
		KartAnimInstanceClass = ABP_KartAnimClass.Class;

		Super::SetAnimInstanceClass(KartAnimInstanceClass);
	}
	
	SetRelativeLocation({0, 0, 10});
	SetRelativeRotation(FRotator(0, -90, 0));
	SetRelativeScale3D(FVector(90.0f));
	Super::SetAllMassScale(0.01);
	Super::SetEnableGravity(false);
	Super::SetSimulatePhysics(false);
	Super::SetMassOverrideInKg(NAME_None, 0.0f, true);

	Super::SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void UKartSkeletalMeshComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UKartSkeletalMeshComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Kart->GetRootBox();
	}
}

void UKartSkeletalMeshComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
