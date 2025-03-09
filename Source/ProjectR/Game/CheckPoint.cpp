#include "CheckPoint.h"

#include "RaceGameState.h"
#include "Components/BoxComponent.h"
#include "ProjectR/Player/RiderPlayerState.h"


ACheckPoint::ACheckPoint()
{
	CheckBox = CreateDefaultSubobject<UBoxComponent>("Check Box");
	CheckBox->SetupAttachment(GetRootComponent());
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMoveToCheckPoint);
}

void ACheckPoint::OnMoveToCheckPoint(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* PlayerKart = Cast<APawn>(OtherActor);
	if (!PlayerKart)
	{
		return;
	}

	ARiderPlayerState* RiderPlayerState = PlayerKart->GetPlayerState<ARiderPlayerState>();

	if (!RiderPlayerState)
	{
		return;
	}

	const bool IsNextCheckPoint = RiderPlayerState->GetCurrentKartCheckPoint() + 1 == CurrentCheckPoint;
	const bool IsNextLap = RiderPlayerState->GetCurrentKartCheckPoint() == GetWorld()->GetGameState<ARaceGameState>()->GetMaxCheckPoint()
			&& CurrentCheckPoint == 0;
	
	if (IsNextCheckPoint)
	{
		RiderPlayerState->SetCheckPoint(CurrentCheckPoint);
		return;
	}

	if (IsNextLap)
	{
		RiderPlayerState->SetCheckPoint(CurrentCheckPoint);
		RiderPlayerState->GoNextRap();
	}
}
