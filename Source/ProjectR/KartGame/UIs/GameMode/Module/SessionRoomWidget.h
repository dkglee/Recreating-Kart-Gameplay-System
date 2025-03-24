#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionRoomWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTR_API USessionRoomWidget : public UUserWidget
{
	GENERATED_BODY()

public: 
	void InitializeData(const FOnlineSessionSearchResult& SearchResult);
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RoomId;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RoomTitle;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentPlayer;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxPlayer;
};
