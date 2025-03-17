// Fill out your copyright notice in the Description page of Project Settings.


#include "KartNetworkSyncComponent.h"

#include "Kart.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UKartNetworkSyncComponent::UKartNetworkSyncComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UKartNetworkSyncComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartNetworkSyncComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
	}
}

void UKartNetworkSyncComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKartNetworkSyncComponent, KartTransform);
}

// Called every frame
void UKartNetworkSyncComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// 클라이언트(자기 자신)는 서버에 RPC를 호출하여 자신의 위치와 회전값을 전달한다.
	if (Kart->IsLocallyControlled() && !Kart->HasAuthority())
	{
		// 서버로 본인의 위치와 회전값을 전달한다.
		Server_SendKartTransform(KartBody->GetComponentTransform());
	}
	else if (Kart->IsLocallyControlled() && Kart->HasAuthority())
	{
		// 서버는 자신의 위치와 회전값을 업데이트한다.
		KartTransform = KartBody->GetComponentTransform();
	}
}

void UKartNetworkSyncComponent::Server_SendKartTransform_Implementation(FTransform InKartTransform)
{
	KartBody->SetWorldTransform(InKartTransform);

	KartTransform = InKartTransform;
}

void UKartNetworkSyncComponent::OnRep_ServerTransform()
{
	// Local Role : Simulated Proxy
	// Remote Role : Authority 
	if (!Kart->IsLocallyControlled())
	{
		// 서버로부터 받은 위치와 회전값을 적용한다.
		KartBody->SetWorldTransform(KartTransform);

		// FString DebugString = FString::Printf(TEXT("KartTransform: %s"), *KartTransform.ToString());
		// DrawDebugString(GetWorld(), Kart->GetActorLocation(), DebugString, nullptr, FColor::Red, 0.0f, true);
	}
}

