#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/PlayerState.h"
#include "SessionPlayerState.generated.h"

UCLASS()
class PROJECTR_API ASessionPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	GETTER(FString, PlayerDisplayName)
	
protected:
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
private:
	UPROPERTY(Replicated)
	FString PlayerDisplayName;
};
