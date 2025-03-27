#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankingResultWidget.generated.h"

class ARiderPlayerState;
class UTextBlock;

UCLASS()
class PROJECTR_API URankingResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeData(ARiderPlayerState* PS);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Rank;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerName;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RunningTime;
};
