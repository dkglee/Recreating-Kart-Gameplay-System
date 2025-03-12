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

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|List", meta = (AllowPrivateAccess = true))
	TSubclassOf<URealTimeRankingHUDItem> RankItemClass;

	UPROPERTY()
	TArray<TObjectPtr<URealTimeRankingHUDItem>> RankListItem;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> RankListOverlay;
	
	UFUNCTION()
	void OnChangedRanking(TArray<APlayerState*> PlayerList);
};
