// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectR/KartGame/Items/DataTable/ItemTable.h"
#include "ItemInventoryComponent.generated.h"



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UItemInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemInventoryComponent();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 아이템 획득
	void GetItem(const FItemTable itemData);

	UFUNCTION(Server, Reliable)
	void Server_GetItem(const FItemTable itemData);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_GetItem(const FItemTable itemData);

	// 아이템 사용
	void UseItem();

	UFUNCTION(Server, Reliable)
	void Server_UseItem();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_UseItem();

	// 아이템 스폰 함수
	void SpawnItem(const FItemTable itemData);
	
	// 조준형 아이템 함수
	void LockPlayer();

	void FindTargetAndTakeAim(FVector start, FVector end, FVector boxHalfSize);

	UFUNCTION(Server, Reliable)
	void Server_FindTarget(FVector start, FVector end, FVector boxHalfSize);
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_TakeAim(FVector start, FVector end, FVector boxHalfSize, FColor BoxColor);

	void DrawAimLineBox(FVector start, FVector end, FVector boxHalfSize, FColor BoxColor);

	void SetUsingAimLocation();

	UFUNCTION(Server, Reliable)
	void Server_CalcAimLocation(class UWidgetComponent* aim);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetUsingAimLocation(class UWidgetComponent* aim, bool bIsWorldPos, FVector pos, FVector scale);

	void ChangeAimColor(bool bIsLockOn);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	class AKart* LockedTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim", meta = (AllowPrivateAccess = "true"))
	class AKart* FinalTarget = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_UseItem = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FItemTable> Inventory;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySpace = 2;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bInventoryIsFull = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	float MaxLockOnDist = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim")
	float InitialBoxSizeOffset = 7.0f;

	// 에임 UI 변수
	FVector InitialAimUIPos;
	FVector InitialAimUIScale;

};
