#pragma once

#include "EnumUtil.generated.h"

UENUM()
enum class ELobbyUI : uint8
{
	GameModeList,
	SessionRoom,
	Popup, // 팝업은 무조건 1개만 뜬다는 가정하에 만든 이름
};

UENUM()
enum class EMatchType : uint8
{
	Undefined,
	Item,
	Speed
};
