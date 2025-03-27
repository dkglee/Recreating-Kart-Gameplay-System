#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetControlComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTriggerInputKey_Notified);

enum class EUIKey : uint8;

class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UWidgetControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWidgetControlComponent();

#pragma region InputDelegate
	FOnTriggerInputKey_Notified OnTriggerInputKey_C_Notified;
	FOnTriggerInputKey_Notified OnTriggerInputKey_F5_Notified;
	FOnTriggerInputKey_Notified OnTriggerInputKey_Esc_Notified;
#pragma endregion

	void PushWidgetStack(const EUIKey& LobbyUIKey);
	void PopWidgetStack();
	void Initialize(UUserWidget* NewBaseWidget);
	void AddWidgetData(const EUIKey Key, UUserWidget* Value);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> WidgetStack;
	
	UPROPERTY()
	TMap<EUIKey, TObjectPtr<UUserWidget>> WidgetData;

	UPROPERTY()
	TObjectPtr<UUserWidget> BaseWidget;
	
#pragma region Input
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input|Key", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_C;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input|Key", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_F5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Input", meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> IA_Esc;
	
	void OnTrigger_C();
	void OnTrigger_F5();
	void OnTrigger_Esc();
	
#pragma endregion
};