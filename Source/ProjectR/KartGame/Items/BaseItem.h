// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()

class PROJECTR_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime) override;
	
public:
	void InitComponents();

	GETTER_SETTER(class AKart*, OwningPlayer);
	
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class AKart* OwningPlayer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UBoxComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	class UStaticMeshComponent* Mesh;	
	
};
