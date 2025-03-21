#include "KartCollisionComponent.h"

#include "Kart.h"
#include "KartFrictionComponent.h"
#include "KartNetworkSyncComponent.h"
#include "Components/BoxComponent.h"
#include "KartGame/Items/ItemBox.h"
#include "KartGame/Games/Objects/CheckPoint.h"
#include "Net/UnrealNetwork.h"

UKartCollisionComponent::UKartCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

}

void UKartCollisionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(UKartCollisionComponent, CurrentCollisionCooldownFrame)
}

void UKartCollisionComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Kart = Cast<AKart>(GetOwner());
}

void UKartCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentCollisionCooldownFrame = 0;
	Kart->GetRootBox()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCollisionKart);	
}

void UKartCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentCollisionCooldownFrame > 0)
	{
		// 줄어든 이후에만 확인 처리하기
		CurrentCollisionCooldownFrame -= 1;

		if (CurrentCollisionCooldownFrame == 0)
		{
			Kart->GetFrictionComponent()->RollbackFriction();
		}
	}
}

// Collision 처리는 서버에서만 진행하고 클라로 위치 보간을 요청시킨다.
void UKartCollisionComponent::OnCollisionKart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Kart->HasAuthority())
	{
		return;
	}

	if (CurrentCollisionCooldownFrame > 0)
	{
		return;
	}

	if (OtherActor->IsA(AItemBox::StaticClass()) ||
		OtherActor->IsA(ACheckPoint::StaticClass()))
	{
		return;
	}
	
	CurrentCollisionCooldownFrame = BaseCollisionCooldownFrame;
	Kart->ClearAcceleration();
	Kart->GetFrictionComponent()->SetInFrictionGripCoeff(0);
	const FVector KartImpulse = Kart->GetNetworkSyncComponent()
				->GetKartInfo().Velocity;
	
	// 카트인 경우는 서로와의 충돌 처리
	if (AKart* OtherKart = Cast<AKart>(OtherActor))
	{
		const FVector OtherKartImpulse = OtherKart->GetNetworkSyncComponent()
			->GetKartInfo().Velocity;
		
		Kart->GetRootBox()->AddImpulse(OtherKartImpulse * -1 * CollisionPower);
		OtherKart->GetKartCollisionComponent()->CurrentCollisionCooldownFrame = BaseCollisionCooldownFrame;
		OtherKart->GetFrictionComponent()->SetInFrictionGripCoeff(0);
		OtherKart->GetRootBox()->AddImpulse(KartImpulse * -1 * CollisionPower);
	} else
	{
		Kart->GetRootBox()->AddImpulse(KartImpulse * -1 * CollisionPower);
	}
}