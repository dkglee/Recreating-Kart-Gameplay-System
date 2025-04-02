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

public:
	void InitializeData();
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	bool IsInitialized = false;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTrackInfo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxTrackInfo;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentTime;

	UFUNCTION()
	void UpdateCurrentTrack(const uint8 CurrentTrack);
};
