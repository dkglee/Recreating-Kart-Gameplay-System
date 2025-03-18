#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameStartCinematicUI.generated.h"

class UImage;
class UMediaSource;
class UMediaPlayer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCinematicEndNotified);

UCLASS()
class PROJECTR_API UGameStartCinematicUI : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnCinematicEndNotified OnCinematicEndNotified;
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
private:
	// 비디오 관련
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMediaPlayer> StartCinematic;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMediaSource> StartCinematicSource;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> CinematicEndNearAnimation;
	
	bool IsStartAnimationWhenCinematicEnd = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Cinematic", meta = (AllowPrivateAccess = true))
	float RemainTimeToStartAnimationWhenCinematicEnd = 3.f;
	
	FWidgetAnimationDynamicEvent OnAnimationEndNotified;

	UFUNCTION()
	void OnCinematicEnd();
};
