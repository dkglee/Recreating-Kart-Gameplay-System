#include "RankingResultUI.h"

#include "RankingResultWidget.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameStateBase.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

void URankingResultUI::InitializeRanking()
{
	Algo::HeapSort(GetWorld()->GetGameState()->PlayerArray, [](const APlayerState* A, const APlayerState* B)
	{
		if (!IsValid(A) || !IsValid(B))
		{
			return true;
		}
		
		const ARiderPlayerState* PSA = Cast<ARiderPlayerState>(A);
		const ARiderPlayerState* PSB = Cast<ARiderPlayerState>(B);

		return PSA->GetRanking() < PSB->GetRanking();
	});

	for (TObjectPtr<APlayerState> PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		ARiderPlayerState* PS = Cast<ARiderPlayerState>(PlayerState);
		
		URankingResultWidget* RankingResultWidget
			= CreateWidget<URankingResultWidget>(this, RankingResultWidgetClass);
		RankingResultWidget->InitializeData(PS);
		
		RankingList->AddChild(RankingResultWidget);
	}
}