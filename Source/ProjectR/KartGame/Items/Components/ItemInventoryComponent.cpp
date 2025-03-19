// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "FastLogger.h"
#include "Kart.h"
#include "Components/WidgetComponent.h"
#include "KartGame/Games/Modes/Race/RacePlayerController.h"
#include "KartGame/Items/Booster/Booster.h"
#include "KartGame/Items/Missile/Missile.h"
#include "KartGame/Items/WaterBomb/WaterBomb.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/HUD/ItemInventory/ItemInventory.h"
#include "Net/UnrealNetwork.h"


UItemInventoryComponent::UItemInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	SetIsReplicatedByDefault(true);
	
	ConstructorHelpers::FObjectFinder<UInputAction> ia_useitem(TEXT("'/Game/Kart/Input/InputAction/IA_UseItem.IA_UseItem'"));
	if (ia_useitem.Succeeded())
	{
		IA_UseItem = ia_useitem.Object;
	}
}

void UItemInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitialAimUIPos = Kart->GetUsingAimComponent()->GetRelativeLocation();
	InitialAimUIScale = Kart->GetUsingAimComponent()->GetRelativeScale3D();
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

void UItemInventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemInventoryComponent, bInventoryIsFull);
	DOREPLIFETIME(UItemInventoryComponent, Inventory);
	DOREPLIFETIME(UItemInventoryComponent, LockedTarget);
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

void UItemInventoryComponent::GetItem(const FItemTable itemData)
{
	if (bInventoryIsFull)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory is Full!"));
		return;
	}
	
	Server_GetItem(itemData);
}

void UItemInventoryComponent::Server_GetItem_Implementation(const FItemTable itemData)
{
	NetMulticast_GetItem(itemData);
}

void UItemInventoryComponent::NetMulticast_GetItem_Implementation(const FItemTable itemData)
{
	Inventory.Add(itemData);
	
	ARacePlayerController* pc = Cast<ARacePlayerController>(Kart->GetController());
	if (pc)
	{
		UMainUI* mainUI = pc->GetMainHUD();
		if (mainUI)
		{
			UItemInventory* inventoryUI = mainUI->GetWBP_ItemInventory();
			if (inventoryUI)
			{
				inventoryUI->GetItemUI(itemData.ItemImage);
			}
		}
	}
	
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

	Server_UseItem();
}

void UItemInventoryComponent::Server_UseItem_Implementation()
{
	NetMulticast_UseItem();
}

void UItemInventoryComponent::NetMulticast_UseItem_Implementation()
{
	const FItemTable usingItem = Inventory[0];
	SpawnItem(usingItem);

	ARacePlayerController* pc = Cast<ARacePlayerController>(Kart->GetController());
	if (pc)
	{
		UMainUI* mainUI = pc->GetMainHUD();
		if (mainUI)
		{
			UItemInventory* inventoryUI = mainUI->GetWBP_ItemInventory();
			if (inventoryUI)
			{
				inventoryUI->UseItemUI();
			}
		}
	}

	if (usingItem.ItemType)
	{
		if (Kart->GetUsingAimComponent())
		{
			Kart->GetUsingAimComponent()->SetVisibility(false);
		}
	}
	
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
	
	if (Inventory[0].ItemType == false)
	{
		return;
	}

	FVector start = Kart->GetRootComponent()->GetComponentLocation();
	FVector end = Kart->GetRootComponent()->GetComponentLocation() + Kart->GetRootComponent()->GetForwardVector() * MaxLockOnDist;

	FVector BoxHalfSize(FVector(100.f,100.f,50.f));
	FindTargetAndTakeAim(start, end, BoxHalfSize);
}

void UItemInventoryComponent::SpawnItem(const FItemTable itemData)
{
	// 아이템 스폰
	FTransform itemTransform;
	itemTransform.SetLocation(Kart->GetActorLocation() + Kart->GetActorForwardVector() * 1000.0f);
	itemTransform.SetRotation(Kart->GetActorRotation().Quaternion());
	itemTransform.SetScale3D(FVector(1.0f));
	DrawDebugString(GetWorld(), Kart->GetActorLocation(), TEXT("spawn Item"), 0, FColor::Red, 0, 1);

	switch (itemData.ItemName)
	{
	case EItemName::Missile:
		{
			if (LockedTarget != nullptr)
			{
				itemTransform.SetLocation(Kart->GetActorLocation() + Kart->GetActorForwardVector() * 100.0f);
				auto* missile = GetWorld()->SpawnActor<AMissile>(itemData.ItemClass, itemTransform);
				missile->SetLockOnPlayer(LockedTarget);
				LockedTarget = nullptr;
			}
			else if (LockedTarget == nullptr)
			{
				FFastLogger::LogConsole(TEXT("조준 타겟이 nullptr입니다"));
			}
			break;
		}
	case EItemName::WaterBomb:
		{
			GetWorld()->SpawnActor<AWaterBomb>(itemData.ItemClass, itemTransform);
			break;
		}
	case EItemName::Booster:
		{
			GetWorld()->SpawnActor<ABooster>(itemData.ItemClass, itemTransform);
			break;
		}
	default:
		break;
	}
}

void UItemInventoryComponent::FindTargetAndTakeAim(FVector start, FVector end, FVector boxHalfSize)
{
	Server_FindTarget(start, end, boxHalfSize);
}

void UItemInventoryComponent::Server_FindTarget_Implementation(FVector start, FVector end, FVector boxHalfSize)
{
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FinalTarget = nullptr;
	// 앞에 있는 카트 중에 가장 가까운 카트를 찾는 로직
	float ClosestDistance = TNumericLimits<float>::Max();

	TArray<FHitResult> InitialHitResults;
	FVector InitialBoxSize = boxHalfSize;
	InitialBoxSize.Y *= 4.0f;

	bool bInitialHit = GetWorld()->SweepMultiByChannel(
		InitialHitResults,
		start,
		end,
		Kart->GetRootComponent()->GetComponentQuat(),
		ECC_Pawn,
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
		ECC_Pawn,
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
			else
			{
				BoxColor = FColor::Green;
				LockedTarget = nullptr;
			}
		}
	}
	else
	{
		BoxColor = FColor::Green;
		LockedTarget = nullptr;
	}
	
	NetMulticast_TakeAim(start, end, AdjustedBoxHalfSize, BoxColor);
}

void UItemInventoryComponent::NetMulticast_TakeAim_Implementation(FVector start, FVector end,
	FVector boxHalfSize, FColor BoxColor)
{
	DrawAimLineBox(start, end, boxHalfSize, BoxColor);

	if (Kart->IsLocallyControlled())
	{
		if (Kart->GetUsingAimComponent())
		{
			Kart->GetUsingAimComponent()->SetVisibility(true);
			SetUsingAimLocation();
		}
	}
}

void UItemInventoryComponent::DrawAimLineBox(FVector start, FVector end, FVector boxHalfSize, FColor BoxColor)
{
	// 서버만 보이도록 설정
	if (Kart->HasAuthority() == false) return;
	
	int NumSteps = 10;
	for (int i = 0; i <= NumSteps; i++)
	{
		float Alpha = (float)i / NumSteps;
		FVector DebugLocation = FMath::Lerp(start, end, Alpha);
	
		DrawDebugBox(GetWorld(), DebugLocation, boxHalfSize, Kart->GetRootComponent()->GetComponentQuat(), BoxColor, false, 0.1f);
	}
}

void UItemInventoryComponent::SetUsingAimLocation()
{
	if (Kart->IsLocallyControlled() == false) return;

	auto* aim = Kart->GetUsingAimComponent();

	if (aim)
	{
		// 조준 성공했을 때는 에임 고정
		if (LockedTarget != nullptr)
		{
			
			aim->SetWorldLocation(LockedTarget->GetTargetAimComponent()->GetComponentLocation());
			aim->SetRelativeScale3D(FVector(0.4f));
		}
		// 조준 실패 중일 때는 에임을 정면 방향으로 설정
		else
		{
			// 주시 중인 상대가 없으면 내 카트 정면에 에임 위치
			if (FinalTarget == nullptr)
			{
				aim->SetRelativeLocation(InitialAimUIPos);
				aim->SetRelativeScale3D(InitialAimUIScale);
			}
			// 에임의 거리는 현재 주시중인 타겟이 내 앞 방향에 가까워짐에 비례하게 에임의 거리가 멀어진다.
			else
			{
				FVector forward = Kart->GetActorForwardVector();
				FVector rightVector = Kart->GetActorRightVector();
				FVector targetLocation = FinalTarget->GetActorLocation();
				FVector kartLocation = Kart->GetActorLocation();

				FVector direction = (targetLocation - kartLocation).GetSafeNormal();
				float facingDot = FVector::DotProduct(forward, direction);
				float rightDot = FVector::DotProduct(rightVector, direction);
				//FFastLogger::LogConsole(TEXT("%f"),facingDot);

				float distance = FVector::Distance(kartLocation, targetLocation);
				float maxDistance = MaxLockOnDist;
				float minDistance = 0.0f;
				float distanceFactor = FMath::Clamp((distance - minDistance) / (maxDistance - minDistance), 0.f, 1.f);

				// 적절한 위치 보정
				// 좌우 확인
				// 0.2는 이동 강도 조절을 위한 값
				FVector lateralOffset = rightVector * rightDot * distance * 0.2f;
				// 정면 확인
				// 0.5는 이동 강도 조절을 위한 값
				FVector forwardOffset = forward * FMath::Clamp(facingDot, 0.0f, 1.0f) * distance * 0.5f;

				FVector newPos = kartLocation + forwardOffset + lateralOffset;
				newPos.Z = InitialAimUIPos.Z; // Z 값 유지

				aim->SetWorldLocation(newPos);
			}
		}
	}
}
