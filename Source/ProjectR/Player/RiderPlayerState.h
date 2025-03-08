#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RiderPlayerState.generated.h"

UCLASS()
class PROJECTR_API ARiderPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void SetCheckPointToNext();
	
private:
	uint16 CurrentKartCheckPoint = 0;
};
