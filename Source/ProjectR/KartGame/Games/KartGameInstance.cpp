#include "KartGameInstance.h"

#include "FastLogger.h"
#include "OnlineSubsystem.h"

void UKartGameInstance::Init()
{
	Super::Init();

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if (!OnlineSubsystem)
	{
		return;
	}

	FFastLogger::LogScreen(FColor::Red, TEXT("하이 세션: %s"), *OnlineSubsystem->GetSubsystemName().ToString());
}
