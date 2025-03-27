#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrackInfo.generated.h"

class ARaceGameState;
class UTextBlock;

UCLASS()
class PROJECTR_API UTrackInfo : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<ARaceGameState> RaceGameState;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTrackInfo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTrackInfo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTime;

	UFUNCTION()
	void UpdateCurrentTrack(const uint8 CurrentTrack);
};
