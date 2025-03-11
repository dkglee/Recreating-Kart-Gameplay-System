#pragma once

#define GETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE type Get##varName() { return varName; }

#define SETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE void Set##varName(type val) { varName = val; }

#define GETTER_SETTER(type, varName) \
UFUNCTION(BlueprintCallable) \
FORCEINLINE type Get##varName() { return varName; } \
UFUNCTION(BlueprintCallable) \
FORCEINLINE void Set##varName(type val) { varName = val; }

class FCommonUtil
{
public:
	template <typename EnumType>
	static FString GetClassEnumKeyAsString(const EnumType EnumeratorValue);
};
