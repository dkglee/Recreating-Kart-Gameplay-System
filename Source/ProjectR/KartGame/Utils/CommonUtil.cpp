#include "CommonUtil.h"

template <typename EnumType>
FString FCommonUtil::GetClassEnumKeyAsString(const EnumType EnumeratorValue)
{
	TArray<FString> EnumKey;
	UEnum::GetValueAsString(EnumeratorValue).
		ParseIntoArray(EnumKey, TEXT("::"));

	if (EnumKey.Num() == 1)
	{
		return EnumKey[0];
	}
	return EnumKey[1];
}
