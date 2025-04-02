#include "KartGameInstance.h"

#include "SessionUtil.h"

void UKartGameInstance::Init()
{
	Super::Init();
	
	FSessionUtil::Init();

	FCoreDelegates::OnExit.AddUObject(this, &ThisClass::ClearSteam);
}

void UKartGameInstance::ClearSteam()
{
	FSessionUtil::Clear();
}

