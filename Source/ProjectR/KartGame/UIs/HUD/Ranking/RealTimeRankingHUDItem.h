#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RealTimeRankingHUDItem.generated.h"

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
	TObjectPtr<ARiderPlayerState> RiderPlayerState;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RankView;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RiderName;
};
