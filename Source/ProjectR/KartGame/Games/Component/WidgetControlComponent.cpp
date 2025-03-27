#include "WidgetControlComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnumUtil.h"
#include "Blueprint/UserWidget.h"
#include "KartGame/UIs/_Common/WidgetStackInterface.h"


UWidgetControlComponent::UWidgetControlComponent()
{
}

void UWidgetControlComponent::Initialize(UUserWidget* NewBaseWidget)
{
	BaseWidget = NewBaseWidget;
	BaseWidget->AddToViewport();
	
	if (IWidgetStackInterface* WidgetStackInterface = Cast<IWidgetStackInterface>(NewBaseWidget))
	{
		WidgetStackInterface->SetDefaultWidgetInfo();
	}
	
	UEnhancedInputComponent* EnhancedInputComponent
		= Cast<UEnhancedInputComponent>(GetOwner()->InputComponent);
	
	if (!EnhancedInputComponent)
	{
		return;
	}

	EnhancedInputComponent->BindAction(IA_Esc, ETriggerEvent::Triggered, this,
		&ThisClass::OnTrigger_Esc);
	EnhancedInputComponent->BindAction(IA_C, ETriggerEvent::Triggered, this,
		&ThisClass::OnTrigger_C);
	EnhancedInputComponent->BindAction(IA_F5, ETriggerEvent::Triggered, this,
		&ThisClass::OnTrigger_F5);
}

void UWidgetControlComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UWidgetControlComponent::AddWidgetData(const EUIKey Key, UUserWidget* Value)
{
	WidgetData.Add(Key, Value);
	Value->SetVisibility(ESlateVisibility::Hidden);
	Value->AddToViewport();
}

void UWidgetControlComponent::PushWidgetStack(const EUIKey& LobbyUIKey)
{
	// 이전의 마지막 위젯에 대한 후처리
	// 위젯 스택이 비어있으면 로비이고, 아니라면 마지막 위젯 스택을 가져오게 처리한다.
	UUserWidget* LastWidget = WidgetStack.IsEmpty() ? BaseWidget : WidgetStack.Last();
	// 팝업 노출 시에는 뒤에 UI가 없어지면 안된다.
	LastWidget->SetVisibility(ESlateVisibility::Hidden);
	if (IWidgetStackInterface* WidgetStackInterface = Cast<IWidgetStackInterface>(LastWidget))
	{
		WidgetStackInterface->ClearWidgetInfo();
	}

	// 위젯 새로 넣고 처리하기
	UUserWidget* NewWidget = WidgetData.FindRef(LobbyUIKey);
	WidgetStack.Push(NewWidget);
	NewWidget->SetVisibility(ESlateVisibility::Visible);
	if (IWidgetStackInterface* WidgetStackInterface = Cast<IWidgetStackInterface>(NewWidget))
	{
		WidgetStackInterface->SetDefaultWidgetInfo();
	}
}

void UWidgetControlComponent::PopWidgetStack()
{
	// 새로운 위젯이 들어오기 전의 처리
	if (WidgetStack.Num() == 0)
	{
		// TODO: ESC 화면 노출
		return;
	}

	WidgetStack.Last()->SetVisibility(ESlateVisibility::Hidden);
	if (IWidgetStackInterface* WidgetStackInterface = Cast<IWidgetStackInterface>(WidgetStack.Last()))
	{
		WidgetStackInterface->ClearWidgetInfo();
	}
	
	WidgetStack.Pop();

	// 위젯 빠진 이후에 대한 처리
	if (WidgetStack.Num() == 0)
	{
		BaseWidget->SetVisibility(ESlateVisibility::Visible);
		if (IWidgetStackInterface* WidgetStackInterface
			= Cast<IWidgetStackInterface>(BaseWidget))
		{
			WidgetStackInterface->SetDefaultWidgetInfo();
		}
		return;
	}
	
	if (IWidgetStackInterface* WidgetStackInterface
		= Cast<IWidgetStackInterface>(WidgetStack.Last()))
	{
		WidgetStackInterface->SetDefaultWidgetInfo();
	}
}

void UWidgetControlComponent::OnTrigger_C()
{
	OnTriggerInputKey_C_Notified.Broadcast();
}

void UWidgetControlComponent::OnTrigger_F5()
{
	OnTriggerInputKey_F5_Notified.Broadcast();
}

void UWidgetControlComponent::OnTrigger_Esc()
{
	// Delegate에 바인딩 된 것이 있다면 그것만 처리
	// Pop widget을 하고싶으면 UI 에서 Delegate Binding을 처리할 것
	if (OnTriggerInputKey_Esc_Notified.IsBound())
	{
		OnTriggerInputKey_Esc_Notified.Broadcast();
		return;
	}
	// 기본 동작은 Widget Stack을 Pop 하는 것이다.
	PopWidgetStack();
}