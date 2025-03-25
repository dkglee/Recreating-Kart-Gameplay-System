#pragma once

#include "CoreMinimal.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionRoomWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class PROJECTR_API USessionRoomWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	void InitializeData(const FOnlineSessionSearchResult& SearchResult);

protected:
	virtual void NativeConstruct() override;
	
private:

#pragma region SessionData
	FOnlineSessionSearchResult SessionSearchResult;
#pragma endregion
	
#pragma region Widget
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SessionRoomButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RoomId;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RoomTitle;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentPlayer;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxPlayer;
#pragma endregion
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	
	UFUNCTION()
	void JoinSession();
	
	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type);
};
