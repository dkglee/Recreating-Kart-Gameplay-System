// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/Actor.h"
#include "KartGame/Items/BaseItem.h"
#include "Missile.generated.h"

UCLASS()
class PROJECTR_API AMissile : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMissile();

protected:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnMissileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                           const FHitResult& SweepResult);
	
public:
	SETTER(class AKart*, LockOnPlayer);

	void MovetoTarget(float DeltaTime);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AKart* LockOnPlayer = nullptr;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float speed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float Amplitude = 5.0f; //파동의 높이
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float Frequency = 15.0f; // 파동의 주기
	
	float ElapsedTime = 0;

	float DistanceToTarget = 0;
	
};
