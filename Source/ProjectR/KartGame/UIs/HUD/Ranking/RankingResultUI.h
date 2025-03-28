#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RankingResultUI.generated.h"

class UVerticalBox;
class URankingResultWidget;

UCLASS()
class PROJECTR_API URankingResultUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeRanking();
	
private:
	UPROPERTY(EditAnywhere, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<URankingResultWidget> RankingResultWidgetClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> RankingList;
};
