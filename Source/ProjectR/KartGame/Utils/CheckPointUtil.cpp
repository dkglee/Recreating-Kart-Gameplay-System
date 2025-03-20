#include "CheckPointUtil.h"

void FCheckPointUtil::GetCheckPointPinInfo(const FString& Pin, TArray<uint16>& PinList)
{
	PinList.Empty();
	
	FString Temp;
	// wchar_t -> 확장형 문자 타입으로 OS 별 모든 것을 지원하는
	// FString을 구성하는 문자 요소인 것으로 추정
	for (const wchar_t Char : Pin)
	{
		// 글자가 분기점 : 인 경우에 대한 대응
		if (Char == ':')
		{
			PinList.Add(FCString::Atoi(*Temp));
			Temp = TEXT("");
			continue;
		}
		Temp += Char;
	}
	
	PinList.Add(FCString::Atoi(*Temp));
}