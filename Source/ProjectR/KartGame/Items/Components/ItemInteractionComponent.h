// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	void Interaction(EInteractionType interactionType);

#pragma region GETTER
	GETTER(bool, bIsInteraction);

	GETTER(EInteractionType, CurrentType);
#pragma endregion
	
private:
	UFUNCTION(Client, Reliable)
	void Client_ChangePhysics(bool bEnable);
	
#pragma region MissileFunction
	void MissileHitInteraction();
	
	void MissileInteraction_Move(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_MissileInteraction_Move(FQuat resultQuat, FVector resultPos);
#pragma endregion

#pragma region WaterFunction
	void WaterBombHitInteraction();
#pragma endregion 

#pragma region ShieldFunction
	UFUNCTION(Server, Reliable)
	void Server_CheckShieldUsingTime();
#pragma endregion
	
private:
	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	EInteractionType CurrentType = EInteractionType::None;
	
	UPROPERTY(Replicated, EditAnywhere, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	bool bIsInteraction = false;
public:
	// 미사일 변수
#pragma region MissileVariance
	FVector InitialPos;
	FQuat InitialQuat;

	float MissileKnockbackElapsedTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MissileKnockbackTime = 1.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MissileKnockbackRotationNumber = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float MissileKnockbackHeight = 500.f;
#pragma endregion 
	// 쉴드 변수
#pragma region ShieldVariance
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bShieldOn = false;

	float ShieldElapsedTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	float ShieldTime = 30.f;
#pragma endregion
	
};
