// Fill out your copyright notice in the Description page of Project Settings.


#include "KartDraftComponent.h"

#include "FastLogger.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartNetworkSyncComponent.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
UKartDraftComponent::UKartDraftComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

}

void UKartDraftComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UKartDraftComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
}

void UKartDraftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Kart->IsLocallyControlled() == false) return;
	
	if (bDraftStart == false)
	{
		FVector start = Kart->GetRootComponent()->GetComponentLocation();
		FVector end = Kart->GetRootComponent()->GetComponentLocation() + Kart->GetRootComponent()->GetForwardVector() * DraftDistance;
		Server_FindTarget(start, end, TraceBoxHalfSize);
	}
	else
	{
		AddDraftForce();
	}
}

void UKartDraftComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKartDraftComponent, bDraftStart);
}

void UKartDraftComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(DraftTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void UKartDraftComponent::DrawTraceLineBox(FVector start, FVector end, FVector boxHalfSize, FColor boxColor)
{
	// 서버만 보이도록 설정
	if (Kart->HasAuthority() == false) return;
	
	int NumSteps = 10;
	for (int i = 0; i <= NumSteps; i++)
	{
		float Alpha = (float)i / NumSteps;
		FVector DebugLocation = FMath::Lerp(start, end, Alpha);
	
		DrawDebugBox(GetWorld(), DebugLocation, boxHalfSize, Kart->GetRootComponent()->GetComponentQuat(), boxColor, false, 0.1f);
	}
}

void UKartDraftComponent::Server_FindTarget_Implementation(FVector start, FVector end, FVector boxHalfSize)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	// 앞에 있는 카트 중에 가장 가까운 카트를 찾는 로직
	float ClosestDistance = TNumericLimits<float>::Max();

	TArray<FHitResult> InitialHitResults;
	FVector InitialBoxSize = boxHalfSize;

	bool bInitialHit = GetWorld()->SweepMultiByChannel(
		InitialHitResults,
		start,
		end,
		Kart->GetRootComponent()->GetComponentQuat(),
		ECC_Pawn,
		FCollisionShape::MakeBox(InitialBoxSize),
		Params);

	if (bInitialHit)
	{
		for (const FHitResult& Hit : InitialHitResults)
		{
			AKart* PotentialTarget = Cast<AKart>(Hit.GetActor());
			if (PotentialTarget && PotentialTarget != FinalTarget)
			{
				float Distance = FVector::Distance(start, Hit.ImpactPoint);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					FinalTarget = PotentialTarget;
				}
			}
			else if (FinalTarget == PotentialTarget)
			{
				continue;
			}
			else
			{
				FinalTarget = nullptr;
			}
		}
	}

	FColor boxColor = FColor::Green;
	if (FinalTarget != nullptr)
	{
		boxColor = FColor::Red;
		CheckTraceTime();
	}

	// DrawTraceLineBox(start, end, boxHalfSize, boxColor);
}

void UKartDraftComponent::CheckTraceTime()
{
	if (Kart->HasAuthority() == false) return;

	if (Kart->GetNetworkSyncComponent()->GetKartInfo().Velocity.Size() < 100.f)
	{
		return;
	}

	ElapsedTime += GetWorld()->GetDeltaSeconds();

	if (ElapsedTime >= DraftStartTime && bDraftStart == false)
	{
		NetMulticast_DraftEffect();
		FFastLogger::LogConsole(TEXT("DRAFT!"));
		bDraftStart = true;
		ElapsedTime = 0.f;
		GetWorld()->GetTimerManager().ClearTimer(DraftTimerHandle);
		TWeakObjectPtr<UKartDraftComponent> WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(DraftTimerHandle, [WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->bDraftStart = false;
			}
		}, DraftDuration, false);
	}
}

void UKartDraftComponent::AddDraftForce()
{
	auto* KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
	auto* AccelerationComponent = Cast<UKartAccelerationComponent>(Kart->GetAccelerationComponent());
	
	FVector force = KartBody->GetForwardVector() * KartBody->GetMass() * DraftForce;
    
	for (int32 i = 0; i < AccelerationComponent->GetWheels().Num(); i++)
	{
		FVector location = AccelerationComponent->GetWheels()[i]->GetComponentLocation();
		KartBody->AddForceAtLocation(force, location);
	}
}

void UKartDraftComponent::NetMulticast_DraftEffect_Implementation()
{
	DrawDebugString(GetWorld(), Kart->GetActorLocation() + Kart->GetActorUpVector() * 50.f, TEXT("DRAFT!"), 0, FColor::Green, 1, true, 2);
}
