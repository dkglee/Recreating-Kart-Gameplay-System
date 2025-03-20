#include "KartResetComponent.h"
#include "KartAccelerationComponent.h"
#include "EnhancedInputComponent.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"
#include "KartGame/Games/Objects/CheckPoint.h"
#include "Kismet/KismetSystemLibrary.h"


UKartResetComponent::UKartResetComponent()
{
	RollbackEffectTimeline
	= CreateDefaultSubobject<UTimelineComponent>("Rollback Effect Timeline");
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);

	static ConstructorHelpers::FObjectFinder<UInputAction> IA_TEMP_Rollback
	(TEXT("/Game/Kart/Input/InputAction/IA_Rollback.IA_Rollback"));
	if (IA_TEMP_Rollback.Succeeded())
	{
		IA_Rollback = IA_TEMP_Rollback.Object;
	}
}

void UKartResetComponent::InitializeComponent()
{
	Super::InitializeComponent();
	Kart = Cast<AKart>(GetOwner());

	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UKartResetComponent::SetupInputBinding);
	}
}

void UKartResetComponent::BeginPlay()
{
	Super::BeginPlay();
	
	RollbackEffectCallback.BindDynamic(this, &ThisClass::OnRollbackEffectCallback);
	RollbackEffectFinish.BindDynamic(this, &ThisClass::OnRollbackEffectFinish);
	
	RollbackEffectTimeline->SetLooping(false);
	RollbackEffectTimeline->AddInterpFloat(RollbackEffectCurve, RollbackEffectCallback);
	RollbackEffectTimeline->SetTimelineFinishedFunc(RollbackEffectFinish);
}

void UKartResetComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_Rollback, ETriggerEvent::Triggered,
		this, &UKartResetComponent::OnRollbackInputDetected);
}

void UKartResetComponent::OnRollbackInputDetected()
{
	ARiderPlayerState* PS = Kart->GetPlayerState<ARiderPlayerState>();
	ACheckPoint* PrevCheckPoint = 
		GetWorld()->GetGameState<ARaceGameState>()->
		GetCheckPointData().FindRef(PS->GetCurrentKartCheckPoint());

	// TODO: 정상적인 Mesh 기반의 트랙인 경우에 아래와 같이 Trace로 처리
	// const FVector PrevCheckPointStartLocation = PrevCheckPoint->GetActorLocation();
	// const FVector PrevCheckPointEndLocation = PrevCheckPoint->GetActorLocation() + FVector(0, 0, -3000);
	//
	// TArray<AActor*> IgnoreActorList;
	// IgnoreActorList.Add(GetOwner());
	// FHitResult HitResult;
	//
	// const bool IsDetectedLandscape = UKismetSystemLibrary::LineTraceSingle(GetWorld(), PrevCheckPointStartLocation, PrevCheckPointEndLocation,
	// 	TraceTypeQuery3, false, IgnoreActorList, EDrawDebugTrace::ForDuration, HitResult, true);
	//
	// UE_LOG(LogTemp, Display, TEXT("Test: %s"), *HitResult.Location.ToString());
	
	Kart->SetActorLocation(PrevCheckPoint->GetActorLocation());
	Kart->GetAccelerationComponent()->ResetAcceleration();
}

void UKartResetComponent::OnRollbackEffectCallback(const float Value)
{
	// TODO: 이펙트의 경우 Skeletal Mesh 도입 이후에 사용 가능
	UE_LOG(LogTemp, Display, TEXT("깜빡깜빡 깜빠기 : %f"), Value);
}

void UKartResetComponent::OnRollbackEffectFinish()
{
	Kart->GetRootBox()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}
