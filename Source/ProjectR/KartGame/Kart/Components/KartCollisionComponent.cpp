#include "KartCollisionComponent.h"

#include "Kart.h"
#include "KartFrictionComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


UKartCollisionComponent::UKartCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

}

void UKartCollisionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UKartCollisionComponent, CollisionCooldownFrame)
}

void UKartCollisionComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Kart = Cast<AKart>(GetOwner());
}

void UKartCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	Kart->GetRootBox()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionKart);	
}

void UKartCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CollisionCooldownFrame > 0)
	{
		CollisionCooldownFrame -= 1;

		if (CollisionCooldownFrame == 0)
		{
			Kart->GetRootBox()->SetSimulatePhysics(true);
		}
	}
}

void UKartCollisionComponent::OnCollisionKart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Kart->HasAuthority())
	{
		return;
	}

	if (CollisionCooldownFrame > 0)
	{
		return;
	}

	CollisionCooldownFrame = 10;
	Kart->ClearAcceleration();
	Kart->GetFrictionComponent()->SetFrictionGrip(0);
	Kart->GetRootBox()->SetSimulatePhysics(false);
	
	if (AKart* OtherKart = Cast<AKart>(OtherActor))
	{
		OtherKart->GetKartCollisionComponent()->CollisionCooldownFrame = 10;
	}
}
