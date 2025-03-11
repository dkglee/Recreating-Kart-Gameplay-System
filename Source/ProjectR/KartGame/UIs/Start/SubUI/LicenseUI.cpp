#include "LicenseUI.h"

#include "Components/TextBlock.h"

void ULicenseUI::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (License)
	{
#define LOCTEXT_NAMESPACE "YourNamespace"
		const FText LicenseText = FText::Format(LOCTEXT("License", "“Project Rider uses Unreal® Engine."
			" Unreal® is a trademark or registered trademark of Epic Games, Inc. in the United States of America and elsewhere.”\n"
			"“Unreal® Engine, Copyright 1998 – {0}, Epic Games, Inc. All rights reserved.”"), FText::FromString(FString::FromInt(FDateTime::Now().GetYear())));
#undef LOCTEXT_NAMESPACE

		License->SetText(LicenseText);
	}
}

void ULicenseUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	PlayAnimation(LicenseShowAnimation);
}
