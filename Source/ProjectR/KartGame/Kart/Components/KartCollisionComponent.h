// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KartInfo.h"
#include "Components/ActorComponent.h"
#include "KartCollisionComponent.generated.h"

USTRUCT()
struct FCollision
{
	GENERATED_BODY()

	UPROPERTY()
	FKartInfo KartInfo = FKartInfo();
	UPROPERTY()
	bool bCollisionWithKart = false;
	UPROPERTY()
	FVector CollisionNormal = FVector::ZeroVector;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartCollisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void OnCollisionKart(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(Client, Reliable)
	void ClientRPC_OnCollisionKart(FCollision CollisionInfo);
	
	
	UPROPERTY()
	class AKart* Kart = nullptr;
	UPROPERTY()
	class UBoxComponent* KartBody = nullptr;

	
};
