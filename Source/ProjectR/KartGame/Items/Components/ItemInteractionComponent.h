// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemInteractionComponent.generated.h"

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	None,
	Explosion,
	Water,
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UItemInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UItemInteractionComponent();

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void MissileHitInteraction();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	EInteractionType CurrentType = EInteractionType::None;
	
public:
	bool bIsInteraction = false;

	// 미사일 변수
	FVector InitialPos;
	FQuat InitialQuat;

	float MissileKnockbackElapsedTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MissileKnockbackTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MissileKnockbackRotationNumber = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MissileKnockbackHeight = 500.f;	

	
};
