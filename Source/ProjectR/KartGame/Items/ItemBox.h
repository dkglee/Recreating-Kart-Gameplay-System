// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/ItemTable.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

UCLASS()
class PROJECTR_API AItemBox : public AActor
{
	GENERATED_BODY()

public:
	AItemBox();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	void InitComponents();

	UFUNCTION()
	void ItemBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UFUNCTION()
	void MakeRandomItem(class UItemInventoryComponent* ItemInventoryComponent);

	UFUNCTION(Server, Reliable)
	void Server_MakeRandomItem(class UItemInventoryComponent* ItemInventoryComponent);

	UFUNCTION(NetMulticast, Reliable)
	void NetMultiCast_MakeRandomItem(class UItemInventoryComponent* ItemInventoryComponent, const FItemTable Item);

	void RotateBody();
private:
	TMap<int, FItemTable> ItemMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
	int32 TotalWeight = 0;

	FTimerHandle ItemBoxRespawnTimerHandle;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UBoxComponent* Root;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UStaticMeshComponent* Mesh;
};
