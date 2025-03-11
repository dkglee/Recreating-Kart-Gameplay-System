#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartUI.generated.h"

class UImage;
class UMediaSource;
class UMediaPlayer;

UCLASS()
class PROJECTR_API UGameStartUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	// 비디오 관련
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMediaPlayer> StartCinematic;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMediaSource> StartCinematicSource;

	bool IsStartAnimationWhenCinematicEnd = false;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CinematicEndNearAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic", meta = (AllowPrivateAccess = true))
	float RemainTimeToStartAnimationWhenCinematicEnd = 3.f;
};
