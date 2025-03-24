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
	UFUNCTION()
	void OnCollisionKart(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	// UFUNCTION()
	// void OnCollisionKart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(Client, Reliable)
	void ClientRPC_OnCollisionKart(FCollision CollisionInfo);
	
	
	UPROPERTY()
	class AKart* Kart = nullptr;
	UPROPERTY()
	class UBoxComponent* KartBody = nullptr;

	
};
