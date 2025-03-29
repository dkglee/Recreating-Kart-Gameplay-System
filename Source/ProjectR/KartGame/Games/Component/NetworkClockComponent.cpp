// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkClockComponent.h"

#include "FastLogger.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"


// Sets default values for this component's properties
UNetworkClockComponent::UNetworkClockComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UNetworkClockComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UNetworkClockComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 매틱마다 클라이언트가 직접 시간을 업데이트 한다.
	// 서버에서 시간을 한번 동기화 처리 된 이후 직접 계산을 함.
	// 서버에서 동기화 될 때 네트워크 지연을 고려하여 세팅해주기 때문에 이후에 클라이언트에서
	// 해당 시간을 계속해서 업데이트 해주면 완전히 일치한 시간을 가질 수 있다.
	ServerTime += DeltaTime;
}

void UNetworkClockComponent::RequestServerTime(APlayerController* Requester)
{
	if (Requester->GetLocalRole() < ROLE_Authority)
	{
		// 클라이언트가 서버에게 시간을 요청한다.
		ServerRPC_RequestServerTime(Requester, GetWorld()->GetTimeSeconds());
	}
}

void UNetworkClockComponent::ServerRPC_RequestServerTime_Implementation(APlayerController* Requester, float RequestTime)
{
	float InServerTime = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();

	FFastLogger::LogConsole(TEXT("Player %s Requested Server Time: %f"), *Requester->GetName(), ServerTime);
	
	ClientRPC_ReceiveServerTime(InServerTime, RequestTime);
}

void UNetworkClockComponent::ClientRPC_ReceiveServerTime_Implementation(float InServerTime, float RequestTime)
{
	// 서버로부터 받은 시간과 클라이언트가 요청한 시간을 비교하여 네트워크 지연을 계산한다.

	// 현재 시간과 요청했던 시간의 차이를 계산한다.
	// 이것이 네트워크 왕복 시간이 됨.
	float RoundTrip = GetWorld()->GetTimeSeconds() - RequestTime;
	// 여기서 2를 나누는 이유는 왕복 시간의 절반만큼 시간을 조정해주기 위함이다.
	// 즉, 서버에서 클라이언트로 오는 편도 시간을 계산하기 위함
	float AdjustTime = InServerTime + RoundTrip / 2.0f;
	
	ServerTime = AdjustTime;
}

