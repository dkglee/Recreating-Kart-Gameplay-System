// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KartGame/Items/BaseItem.h"
#include "WaterBomb.generated.h"

UCLASS()
class PROJECTR_API AWaterBomb : public ABaseItem
{
	GENERATED_BODY()

public:
	AWaterBomb();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnWaterBombBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);

public:
	void MoveToEstimateLocation(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_MoveToEstimateLocation(FVector resultPos);

	void SetWaterBombScale(float DeltaTime);
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetWaterBombScale(FVector resultScale);

private:
	// 이동 변수
	float MoveElapsedTime = 0.f;
	
	FVector StartPos = FVector::ZeroVector;
	
	FVector EndPos = FVector::ZeroVector;
	
	bool bArriveEndPos = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float ThrowingDistance = 6000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float MovementDuration = 1.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float Height = 3000.0f;
	
	// 크기 변수
	float ScaleElapsedTime = 0.f;

	FVector InitialScale = FVector::ZeroVector;

	bool bIsScaleBigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float ScaleDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	FVector BigScale = FVector(10.f);

};
