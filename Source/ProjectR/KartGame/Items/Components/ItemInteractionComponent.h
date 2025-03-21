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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	void MissileHitInteraction();
	
	void MissileInteraction_Move(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void Server_MissileInteraction_Move(float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_MissileInteraction_Move(FQuat resultQuat, FVector resultPos);

	UFUNCTION(Client, Reliable)
	void Client_ChangePhysics(bool bEnable);


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	EInteractionType CurrentType = EInteractionType::None;
	
public:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Interaction")
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
