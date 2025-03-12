#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RealTimeRankingHUDItem.generated.h"

class UTextBlock;

UCLASS()
class PROJECTR_API URealTimeRankingHUDItem : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RankView;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RiderName;
};
