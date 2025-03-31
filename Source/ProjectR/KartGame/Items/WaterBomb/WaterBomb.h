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
	void OnWaterBombBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
#pragma region MovementFunc
	void MoveToEstimateLocation(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_MoveToEstimateLocation(FVector resultPos);
#pragma endregion

#pragma region ChangeScaleFunc
	void SetWaterBombScale(float DeltaTime);
	
	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetWaterBombScale(FVector resultScale);
#pragma endregion
	
private:
	// 이동 변수
#pragma region MovementVariance
	UPROPERTY()
	float MoveElapsedTime = 0.f;

	UPROPERTY()
	FVector StartPos = FVector::ZeroVector;

	UPROPERTY()
	FVector EndPos = FVector::ZeroVector;

	UPROPERTY()
	bool bArriveEndPos = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float ThrowingDistance = 5000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float MovementDuration = 1.0f;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float Height = 3000.0f;
#pragma endregion 
	// 크기 변수
#pragma region ChangeScaleVariance
	UPROPERTY()
	float ScaleElapsedTime = 0.f;

	UPROPERTY()
	FVector InitialScale = FVector::ZeroVector;

	UPROPERTY()
	bool bIsScaleBigger = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	float ScaleDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true"))
	FVector BigScale = FVector(10.f);
#pragma endregion 

};
