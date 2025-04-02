// Fill out your copyright notice in the Description page of Project Settings.


#include "KartDraftComponent.h"

#include "EngineUtils.h"
#include "Kart.h"
#include "KartAccelerationComponent.h"
#include "KartNetworkSyncComponent.h"
#include "KartSuspensionComponent.h"
#include "Components/BoxComponent.h"
#include "Components/RadialSlider.h"
#include "KartGame/Games/Modes/Race/RacePlayerController.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/NotificationUI/NotificationUI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"


UKartDraftComponent::UKartDraftComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

}

void UKartDraftComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* actor = *ActorItr;
		if (Cast<AKart>(actor))
		{
			continue;
		}
		IgnoredActors.Add(*ActorItr);
	}
}

void UKartDraftComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	PC = Cast<ARacePlayerController>(GetWorld()->GetFirstPlayerController());
}

void UKartDraftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Kart->IsLocallyControlled() == false) return;

	if (Kart->GetbCanMove())
	{
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
}

void UKartDraftComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UKartDraftComponent, bDraftStart);
	DOREPLIFETIME(UKartDraftComponent, ElapsedTime);
}

void UKartDraftComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(DraftTimerHandle);
	Super::EndPlay(EndPlayReason);
}

void UKartDraftComponent::DrawTraceLineBox_Implementation(FVector start, FVector end, FVector boxHalfSize,
	FColor boxColor)
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
	Params.AddIgnoredActors(IgnoredActors);
	
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
					//FFastLogger::LogConsole(TEXT("%s"), *Hit.GetActor()->GetName());
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
	else
	{
		FinalTarget = nullptr;
	}

	FColor boxColor = FColor::Green;
	if (FinalTarget != nullptr)
	{
		boxColor = FColor::Red;
		Server_CheckTraceTime();
	}
	else
	{
		ElapsedTime = 0.0f;
		NetMulticast_SetDraftProgressBarValueAndOpacity(0.0f, 1.0f);
	}

	// DrawTraceLineBox(start, end, boxHalfSize, boxColor);
}

void UKartDraftComponent::Server_CheckTraceTime_Implementation()
{
	float forwardSpeed = UKismetMathLibrary::Dot_VectorVector(Kart->GetRootBox()->GetForwardVector(),Kart->GetNetworkSyncComponent()->GetKartInfo().Velocity);
	float KartSpeedKm = forwardSpeed * 0.036;
	int32 DashBoardSpeed = FMath::RoundToInt(KartSpeedKm * 2);

	if (DashBoardSpeed < 80.f)
	{
		ElapsedTime = 0.f;
		return;
	}

	ElapsedTime += GetWorld()->GetDeltaSeconds();
	
	if (ElapsedTime >= DraftStartTime && bDraftStart == false)
	{
		bDraftStart = true;
		NetMulticast_DraftEffect(true);
		ElapsedTime = 0.f;

		
		GetWorld()->GetTimerManager().ClearTimer(DraftTimerHandle);
		TWeakObjectPtr<UKartDraftComponent> WeakThis = this;
		GetWorld()->GetTimerManager().SetTimer(DraftTimerHandle, [WeakThis]()
		{
			if (WeakThis.IsValid())
			{
				WeakThis->NetMulticast_DraftEffect(false);
				WeakThis->bDraftStart = false;
				WeakThis->FinalTarget = nullptr;
				WeakThis->NetMulticast_SetDraftProgressBarValueAndOpacity(0.0f, 1.0f);
			}
		}, DraftDuration, false);
	}
	else if (ElapsedTime < DraftStartTime)
	{
		NetMulticast_SetDraftProgressBarValueAndOpacity(ElapsedTime / DraftStartTime, 1.0f);
	}
}

void UKartDraftComponent::AddDraftForce()
{
	NetMulticast_SetDraftProgressBarValueAndOpacity(1.0f, 1.0f);
	auto* KartBody = Cast<UBoxComponent>(Kart->GetRootComponent());
	auto* AccelerationComponent = Cast<UKartAccelerationComponent>(Kart->GetAccelerationComponent());
	
	FVector force = KartBody->GetForwardVector() * KartBody->GetMass() * DraftForce;
    
	for (int32 i = 0; i < AccelerationComponent->GetWheels().Num(); i++)
	{
		FVector location = AccelerationComponent->GetWheels()[i]->GetComponentLocation();
		KartBody->AddForceAtLocation(force, location);
	}
}

void UKartDraftComponent::NetMulticast_DraftEffect_Implementation(bool value)
{
	if (Kart->IsLocallyControlled() == false) return;
	
	auto* pc = Cast<ARacePlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		pc->GetMainHUD()->GetWBP_NotificationUI()->SetDraftVisibleUI(value);
	}
}

void UKartDraftComponent::NetMulticast_SetDraftProgressBarValueAndOpacity_Implementation(float progBarValue, float opacityValue)
{
	if (Kart->IsLocallyControlled() == false) return;
	
	PC->GetMainHUD()->GetWBP_NotificationUI()->SetDraftProgressBar(progBarValue);

	if (!bDraftStart)
	{
		PC->GetMainHUD()->GetWBP_NotificationUI()->DraftProgressBar->SetRenderOpacity(opacityValue);
	}
}
