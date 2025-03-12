#include "RealTimeRankingHUDItem.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "KartGame/Games/Modes/RiderPlayerState.h"

void URealTimeRankingHUDItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void URealTimeRankingHUDItem::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!RiderPlayerState)
	{
		return;
	}
	
	RankView->SetText(FText::FromString(FString::FromInt(RiderPlayerState->GetRanking())));
	SetPadding(FMargin(0.f, (RiderPlayerState->GetRanking() - 1) * 160, 0.f, 0.f));
}

void URealTimeRankingHUDItem::InitializeData(APlayerState* PS)
{
	ARiderPlayerState* RiderPS = Cast<ARiderPlayerState>(PS);
	if (!RiderPS)
	{
		return;
	}
	
	RiderPlayerState = RiderPS;
	SetPadding(FMargin(0.f, RiderPS->GetRanking() - 1 * GetCachedGeometry().Size.Y, 0.f, 0.f));
	RankView->SetText(FText::FromString(FString::FromInt(RiderPS->GetRanking())));
	RiderName->SetText(FText::FromString(PS->GetName()));
}
