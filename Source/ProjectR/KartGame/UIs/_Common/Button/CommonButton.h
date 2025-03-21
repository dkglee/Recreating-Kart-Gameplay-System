#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "Blueprint/UserWidget.h"
#include "CommonButton.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class PROJECTR_API UCommonButton : public UUserWidget
{
	GENERATED_BODY()

public:
	GETTER(TObjectPtr<UButton>, Root)
	
protected:
	virtual void NativePreConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Root;
	
	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	FString Content;
	
	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	FString InputKey;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ContentBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> InputKeyBox;

};
