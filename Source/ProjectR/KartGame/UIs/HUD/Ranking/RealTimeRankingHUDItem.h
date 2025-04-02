#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RealTimeRankingHUDItem.generated.h"

class UOverlay;
class ARiderPlayerState;
class UTextBlock;

UCLASS()
class PROJECTR_API URealTimeRankingHUDItem : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeData(APlayerState* PS);
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	bool IsInitializedToSetPosition = false;
	UPROPERTY()
	uint8 CurrentRank = 0;
	UPROPERTY()
	float DeltaTimeStack = 0;
	UPROPERTY()
	float OverlayHeight = 0;
	
	UPROPERTY()
	TObjectPtr<ARiderPlayerState> RiderPlayerState;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> RankBoard;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RankView;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RiderName;
};
