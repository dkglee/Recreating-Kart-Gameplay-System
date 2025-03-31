#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RealTimeRankingHUDBoard.generated.h"

class UOverlay;
class URealTimeRankingHUDItem;
class UTextBlock;

UCLASS()
class PROJECTR_API URealTimeRankingHUDBoard : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeData();

	void InitializeRanking();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|List", meta = (AllowPrivateAccess = true))
	TSubclassOf<URealTimeRankingHUDItem> RankItemClass;

	UPROPERTY()
	TArray<TObjectPtr<URealTimeRankingHUDItem>> RankListItem;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> RankListOverlay;

	FTimerHandle InitializeLoopTimer;
};
