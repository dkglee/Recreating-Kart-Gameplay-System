#pragma once

#include "EnumUtil.generated.h"

UENUM()
enum class EUIKey : uint8
{
	GameModeList,
	SessionRoom,
};

UENUM()
enum class EMatchType : uint8
{
	Undefined,
	Item,
	Speed
};

UENUM()
enum class ERaceStatus
{
	Idle,
	Ready,
	Playing,
	HoldToFinish,
	Finish
};
