#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RealTimeRankingHUDBoard.generated.h"

class UTextBlock;

UCLASS()
class PROJECTR_API URealTimeRankingHUDBoard : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	TMap<APlayerState*, UTextBlock*> RankingListView;
	
	UPROPERTY()
	TMap<APlayerState*, uint8> RankViewData;

	UFUNCTION()
	void OnChangedRanking(TArray<APlayerState*> PlayerList);
};
