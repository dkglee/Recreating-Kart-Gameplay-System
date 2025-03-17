// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "FastLogger.h"
#include "Kart.h"
#include "KartGame/Items/Booster/Booster.h"
#include "KartGame/Items/Missile/Missile.h"
#include "KartGame/Items/WaterBomb/WaterBomb.h"


UItemInventoryComponent::UItemInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
	ConstructorHelpers::FObjectFinder<UInputAction> ia_useitem(TEXT("'/Game/Kart/Input/InputAction/IA_UseItem.IA_UseItem'"));
	if (ia_useitem.Succeeded())
	{
		IA_UseItem = ia_useitem.Object;
	}
}

void UItemInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UItemInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Kart = Cast<AKart>(GetOwner());
	if (Kart)
	{
		Kart->OnInputBindingDelegate.AddDynamic(this, &UItemInventoryComponent::SetupInputBinding);
	}
}

void UItemInventoryComponent::SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(IA_UseItem, ETriggerEvent::Ongoing, this, &UItemInventoryComponent::LockPlayer);
	PlayerInputComponent->BindAction(IA_UseItem, ETriggerEvent::Completed, this, &UItemInventoryComponent::UseItem);
}

void UItemInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UItemInventoryComponent::GetItem(const FItemTable* itemData)
{ 
	if (bInventoryIsFull)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Full!"));
		return;
	}

	FFastLogger::LogScreen(FColor::Green, TEXT("GetItem : %s"), *FCommonUtil::GetClassEnumKeyAsString(itemData->ItemName));
	Inventory.Add(itemData);

	if (Inventory.Num() == MaxInventorySpace)
	{
		bInventoryIsFull = true;
	}
}

void UItemInventoryComponent::UseItem()
{
	if (Inventory.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Empty!"));
		return;
	}

	const FItemTable* usingItem = Inventory[0];
	FFastLogger::LogScreen(FColor::Green, TEXT("UsingItem : %s"), *FCommonUtil::GetClassEnumKeyAsString(usingItem->ItemName));


	SpawnItem(usingItem);
	Inventory.RemoveAt(0);
	bInventoryIsFull = false;
}

void UItemInventoryComponent::LockPlayer()
{
	if (Inventory.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Empty!"));
		return;
	}
	
	if (!Inventory[0]->ItemType)
	{
		return;
	}

	FVector start = Kart->GetRootComponent()->GetComponentLocation();
	FVector end = Kart->GetRootComponent()->GetComponentLocation() + Kart->GetRootComponent()->GetForwardVector() * MaxLockOnDist;

	FVector BoxHalfSize(FVector(100.f,100.f,50.f));
	MakeTraceBoxAndCheckHit(start, end, BoxHalfSize);
}

void UItemInventoryComponent::SpawnItem(const FItemTable* itemData)
{
	// 아이템 스폰
	FTransform t;
	t.SetLocation(Kart->GetActorLocation() + Kart->GetActorForwardVector() * 1000.0f);
	t.SetRotation(Kart->GetActorRotation().Quaternion());
	t.SetScale3D(FVector(1.0f));
	switch (itemData->ItemName)
	{
	case EItemName::Missile:
		{
			if (LockedTarget == nullptr)
			{
				FFastLogger::LogConsole(TEXT("lock missed"));
			}
			else
			{
				t.SetLocation(Kart->GetActorLocation() + Kart->GetActorForwardVector() * 100.0f);
				auto* missile = GetWorld()->SpawnActor<AMissile>(itemData->ItemClass, t);
				missile->SetLockOnPlayer(LockedTarget);
			}
			break;
		}
	case EItemName::WaterBomb:
		{
			GetWorld()->SpawnActor<AWaterBomb>(itemData->ItemClass, t);
			break;
		}
	case EItemName::Booster:
		{
			GetWorld()->SpawnActor<ABooster>(itemData->ItemClass, t);
			break;
		}
	default:
			break;
	}
}

void UItemInventoryComponent::MakeTraceBoxAndCheckHit(FVector start, FVector end, FVector boxHalfSize)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());


	// 앞에 있는 카트 중에 가장 가까운 카트를 찾는 로직
	AKart* FinalTarget = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();

	TArray<FHitResult> InitialHitResults;
	FVector InitialBoxSize = boxHalfSize;
	InitialBoxSize.Y *= 4.0f;

	bool bInitialHit = GetWorld()->SweepMultiByChannel(
		InitialHitResults,
		start,
		end,
		Kart->GetRootComponent()->GetComponentQuat(),
		ECC_Visibility,
		FCollisionShape::MakeBox(InitialBoxSize),
		Params);

	if (bInitialHit)
	{
		for (const FHitResult& Hit : InitialHitResults)
		{
			AKart* PotentialTarget = Cast<AKart>(Hit.GetActor());
			if (PotentialTarget)
			{
				float Distance = FVector::Distance(start, Hit.ImpactPoint);
				if (Distance < ClosestDistance)
				{
					ClosestDistance = Distance;
					FinalTarget = PotentialTarget;
				}
			}
		}
	}

	// 박스의 scale y 보간
	float MinYScale = 0.5f;
	float MaxYScale = 2.0f;
	float DistanceScale = MaxYScale;

	if (FinalTarget)
	{
		DistanceScale = FMath::Lerp(MinYScale, MaxYScale, ClosestDistance / MaxLockOnDist);
	}

	FVector AdjustedBoxHalfSize = boxHalfSize;
	AdjustedBoxHalfSize.Y *= DistanceScale;

	// 조정된 박스크기로 실제 트레이스 실행
	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		start,
		end,
		Kart->GetRootComponent()->GetComponentQuat(),
		ECC_Visibility,
		FCollisionShape::MakeBox(AdjustedBoxHalfSize),
		Params);

	FColor BoxColor = FColor::Green;
	if (bHit)
	{
		for (const FHitResult hit : HitResults)
		{
			auto* lockedTarget = Cast<AKart>(hit.GetActor());
			if (lockedTarget)
			{
				BoxColor = FColor::Red;
				LockedTarget = lockedTarget;
			}
		}
	}
	else
	{
		BoxColor = FColor::Green;
		LockedTarget = nullptr;
	}

	int NumSteps = 10;
	for (int i = 0; i <= NumSteps; i++)
	{
		float Alpha = (float)i / NumSteps;
		FVector DebugLocation = FMath::Lerp(start, end, Alpha);
	
		DrawDebugBox(GetWorld(), DebugLocation, AdjustedBoxHalfSize, Kart->GetRootComponent()->GetComponentQuat(), BoxColor, false, 0.1f);
	}
}

