#include "RealTimeRankingHUDItem.h"

#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

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

	// 초기화를 위한 조건
	// 1. 랭크가 0이 아닐 것 (즉 Initialize과정이 거쳐진 것)
	// 2. Overlay의 Height가 계산이 될 것
	const float CalcPaddingTopValue = (CurrentRank - 1) * OverlayHeight;
	
	if (!IsInitializedToSetPosition && CurrentRank != 0 && RankBoard->GetDesiredSize().Y != 0)
	{
		IsInitializedToSetPosition = true;
		OverlayHeight = RankBoard->GetDesiredSize().Y;
		SetPadding(FMargin(0.f, CalcPaddingTopValue, 0.f, 0.f));
		return;
	}
	
	// TODO: 실제는 해당 UI가 게임 로딩을 위해 유저
	CurrentRank = RiderPlayerState->GetRanking();
	RankView->SetText(FText::FromString(FString::FromInt(CurrentRank)));
	const float NewTopPadding =
		FMath::FInterpTo(GetPadding().Top, CalcPaddingTopValue, InDeltaTime, 5);
	SetPadding(FMargin(0.f, NewTopPadding, 0.f, 0.f));
}

void URealTimeRankingHUDItem::InitializeData(APlayerState* PS)
{
	ARiderPlayerState* RiderPS = Cast<ARiderPlayerState>(PS);
	if (!RiderPS)
	{
		return;
	}
	
	RiderPlayerState = RiderPS;
	CurrentRank = RiderPS->GetRanking();
	IsInitializedToSetPosition = false;
	
	RankView->SetText(FText::FromString(FString::FromInt(CurrentRank)));
	RiderName->SetText(FText::FromString(PS->GetPlayerName()));
}
