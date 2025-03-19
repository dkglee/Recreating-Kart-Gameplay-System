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

	void TakeAim(FVector start, FVector end, FVector boxHalfSize);

	UFUNCTION(Server, Reliable)
	void Server_TakeAim(FVector start, FVector end, FVector boxHalfSize);
	
	void TakeAimToFindTarget(FVector start, FVector end, FVector boxHalfSize, class AKart* FinalTarget, float ClosestDistance);

	UFUNCTION(Server, Reliable)
	void Server_TakeAimToFindTarget(FVector start, FVector end, FVector boxHalfSize, class AKart* FinalTarget, float ClosestDistance);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_TakeAimToFindTarget(FVector start, FVector end, FVector boxHalfSize, FColor BoxColor);

	void DrawAimLineBox(FVector start, FVector end, FVector boxHalfSize, FColor BoxColor);

	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class AKart* LockedTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_UseItem = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TArray<FItemTable> Inventory;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int32 MaxInventorySpace = 2;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	bool bInventoryIsFull = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	float MaxLockOnDist = 3000.f;
};
