#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrackInfo.generated.h"

class UTextBlock;

UCLASS()
class PROJECTR_API UTrackInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTrackInfo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTrackInfo;

	UFUNCTION()
	void UpdateCurrentTrack(const uint8 CurrentTrack);
};
