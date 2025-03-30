// Fill out your copyright notice in the Description page of Project Settings.


#include "KartCollisionComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartNetworkSyncComponent.h"
#include "Components/BoxComponent.h"
#include "KartGame/Games/Objects/CheckPoint.h"
#include "KartGame/Items/ItemBox.h"


// Sets default values for this component's properties
UKartCollisionComponent::UKartCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UKartCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UKartCollisionComponent::OnCollisionKart(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(AItemBox::StaticClass()) ||
	OtherActor->IsA(ACheckPoint::StaticClass()))
	{
		return;
	}
	
	FCollision CollisionInfo;
	
	// 충돌이 발생했을 때 발생한 두 개의 클라이언트에게 충돌 로직을 실행하도록 요청한다. // 그냥 자기 자신만 검출하면 되네
	AKart* OtherKart = Cast<AKart>(OtherActor);
	if (OtherKart)
	{
		CollisionInfo.KartInfo = OtherKart->GetNetworkSyncComponent()->GetKartInfo();
		CollisionInfo.bCollisionWithKart = true;
	}
	
	CollisionInfo.CollisionNormal = Hit.ImpactNormal;
	
	if (Kart->IsLocallyControlled())
	{
		ClientRPC_OnCollisionKart_Implementation(CollisionInfo);
	}
	else
	{
		ClientRPC_OnCollisionKart(CollisionInfo);
	}
}

void UKartCollisionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		KartBody = Kart->GetRootBox();
		// 서버에서만 충돌을 감지할 예정
		if (KartBody && Kart->HasAuthority())
		{
			KartBody->OnComponentHit.AddDynamic(this, &UKartCollisionComponent::OnCollisionKart);
			// KartBody->OnComponentBeginOverlap.AddDynamic(this, &UKartCollisionComponent::OnOverlapKart);
		}
	}
}

// Called every frame
void UKartCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UKartCollisionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		GetWorld()->GetTimerManager().ClearTimer(CoolTimeHandle);
		if (KartBody)
		{
			KartBody->OnComponentHit.RemoveDynamic(this, &UKartCollisionComponent::OnCollisionKart);
			KartBody->OnComponentBeginOverlap.RemoveDynamic(this, &UKartCollisionComponent::OnOverlapKart);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void UKartCollisionComponent::OnOverlapKart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if (OtherActor->IsA(AItemBox::StaticClass()) ||
	// OtherActor->IsA(ACheckPoint::StaticClass()))
	// {
	// 	return;
	// }
	//
	// FCollision CollisionInfo;
	//
	// // 충돌이 발생했을 때 발생한 두 개의 클라이언트에게 충돌 로직을 실행하도록 요청한다. // 그냥 자기 자신만 검출하면 되네
	// AKart* OtherKart = Cast<AKart>(OtherActor);
	// if (OtherKart)
	// {
	// 	CollisionInfo.KartInfo = OtherKart->GetNetworkSyncComponent()->GetKartInfo();
	// 	CollisionInfo.bCollisionWithKart = true;
	// }
	//
	// CollisionInfo.CollisionNormal = SweepResult.ImpactNormal;
	//
	// if (Kart->IsLocallyControlled())
	// {
	// 	ClientRPC_OnCollisionKart_Implementation(CollisionInfo);
	// }
	// else
	// {
	// 	ClientRPC_OnCollisionKart(CollisionInfo);
	// }
}

void UKartCollisionComponent::ClientRPC_OnCollisionKart_Implementation(FCollision CollisionInfo)
{
	// 쿨타임 체크
	if (bCoolTime)
	{
		return;
	}

	bCoolTime = true;
	TWeakObjectPtr<UKartCollisionComponent> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(CoolTimeHandle, FTimerDelegate::CreateLambda([WeakThis]()
	{
		if (WeakThis.IsValid())
		{
			UKartCollisionComponent* StrongThis = WeakThis.Get();
			StrongThis->bCoolTime = false;
		}
	}), CoolTime, false);
	
	FVector ImpactNormal = CollisionInfo.CollisionNormal.GetSafeNormal();
	float SlopeAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(ImpactNormal, FVector::UpVector)));

	if (SlopeAngle < 70.f)
	{
		// 평지나 언덕 -> 충돌 무시
		return;
	}
	
	KartBody->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

	Kart->GetAccelerationComponent()->ClearAcceleration();
}

