// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Kart.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*, PlayerInput);

UCLASS()
class PROJECTR_API AKart : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AKart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FInputBindingDelegate OnInputBindingDelegate;
private:
	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputMappingContext* Imc_Kart = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	class UKartMovementComponent* KartMovementComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UKartSteeringComponent* KartSteeringComponent = nullptr;
};
