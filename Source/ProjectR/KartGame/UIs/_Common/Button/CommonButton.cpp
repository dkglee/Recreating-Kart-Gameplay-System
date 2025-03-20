#include "CommonButton.h"

#include "Components/TextBlock.h"

void UCommonButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ContentBox)
	{
		ContentBox->SetText(FText::FromString(Content));
	}

	if (InputKeyBox)
	{
		InputKeyBox->SetText(FText::FromString(InputKey));
	}
}