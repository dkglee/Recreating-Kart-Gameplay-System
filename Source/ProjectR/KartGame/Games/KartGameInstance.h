// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "KartGameInstance.generated.h"

class FOnlineSessionSearch;
class IOnlineSession;

UCLASS()
class PROJECTR_API UKartGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UKartGameInstance();

	// TODO: 추후 커스텀 방을 위한 Parameter 추가 예정
	void CreateNewGameSession();
	void SearchGameSession();
	void JoinGameSession(FOnlineSessionSearchResult& Result);
	
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	
protected:
	virtual void Init() override;

private:
	// ESPMode::ThreadSafe -> 멀티 스레드 환경에서도 공유되도록 설정하는 Key로 인지 중
	// TODO: 정확한 의미 파악하기.
	// TSharedPtr<IOnlineSession, ESPMode::ThreadSafe> OnlineSessionInterface;

	// TODO: 카테고리에 따른 Map 형태로 저장해도 무방해보임
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	// 현재 만들어진 온라인 세션 관리가 필요할 때 사용하는 값
	IOnlineSessionPtr OnlineSessionInterface;

	void OnSessionCreated(FName SessionName, bool IsCreateSuccess);

	void OnFindSession(bool IsSuccess);

	void OnJoinSession(FName SessionName, EOnJoinSessionCompleteResult::Type Type);
};