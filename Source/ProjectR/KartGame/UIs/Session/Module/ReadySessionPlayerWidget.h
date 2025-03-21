#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReadySessionPlayerWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class PROJECTR_API UReadySessionPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void InitializeData(const APlayerState* PS);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> PlayerThumb;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerName;
};
