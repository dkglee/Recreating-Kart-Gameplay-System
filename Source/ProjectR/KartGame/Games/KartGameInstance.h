// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "KartGameInstance.generated.h"

class IOnlineSession;

UCLASS()
class PROJECTR_API UKartGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

private:
	// ESPMode::ThreadSafe -> 멀티 스레드 환경에서도 공유되도록 설정하는 Key로 인지 중
	// TODO: 정확한 의미 파악하기.
	TSharedPtr<IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;
};
