#include "PingManagerComponent.h"

#include "FastLogger.h"
#include "KartGame/Games/KartGameInstance.h"
#include "Net/UnrealNetwork.h"

UPingManagerComponent::UPingManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPingManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPingManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPingManagerComponent, IsPingReceived);
}

void UPingManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetOwner()->HasAuthority())
	{
		PingToServer();
	} else
	{
		Server_PingToServer();
	}
}

void UPingManagerComponent::PingToServer()
{
	if (IsPingReceived)
	{
		return;
	}

	FFastLogger::LogScreen(FColor::Emerald, TEXT("핑 성공 하다: %s"), *GetName());
	
	// 서버의 핑 카운트만 증가
	GetWorld()->GetGameInstance<UKartGameInstance>()->SetPingCount(
		GetWorld()->GetGameInstance<UKartGameInstance>()->GetPingCount() + 1);
	
	IsPingReceived = true;
	PrimaryComponentTick.bCanEverTick = false;

	// 같지않고 더 큰 경우는 중도 참여 즉 개발 환경에 대한 세팅이다.
	// 실제로는 동일한 경우만이 비즈니스 로직 상 맞는 표현이다.
	if (GetWorld()->GetGameInstance<UKartGameInstance>()->GetPingCount()
		>= GetWorld()->GetGameInstance<UKartGameInstance>()->GetInGamePlayerCount())
	{
		FFastLogger::LogScreen(FColor::Emerald, TEXT("게임 시작하다: %s"), *GetName());
		OnAllPingAccessNotified.Broadcast();
	}
}

void UPingManagerComponent::Server_PingToServer_Implementation()
{
	PingToServer();
}
