// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region GetterSetter
	GETTER(class UItemInventoryComponent*, ItemInventoryComponent);
	GETTER(class UItemInteractionComponent*, ItemInteractionComponent);
	GETTER(float, NormalizedSpeed);
	GETTER(class UKartAccelerationComponent*, AccelerationComponent);
	GETTER(class UBoxComponent*, RootBox);
	GETTER(class UKartSuspensionComponent*, LF_Wheel);
	GETTER(class UKartSuspensionComponent*, RF_Wheel);
	GETTER(class UKartSuspensionComponent*, LR_Wheel);
	GETTER(class UKartSuspensionComponent*, RR_Wheel);
	GETTER(class UKartNetworkSyncComponent*, NetworkSyncComponent);
	GETTER(float, MaxSpeed);
	GETTER_SETTER(bool, bCanMove);
#pragma endregion
	
	FInputBindingDelegate OnInputBindingDelegate;

private:
	
#pragma region ForeignVariable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	bool bCanMove = true;
#pragma endregion

#pragma region CoreSkills
	void CalcuateNormalizedSpeed();
	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* Imc_Kart = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BoxCollision", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* RootBox = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SprintArm", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SprintArm", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartSuspensionComponent* LF_Wheel = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartSuspensionComponent* RF_Wheel = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartSuspensionComponent* LR_Wheel = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartSuspensionComponent* RR_Wheel = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartAccelerationComponent* AccelerationComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartSteeringComponent* SteeringComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UItemInventoryComponent* ItemInventoryComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UItemInteractionComponent* ItemInteractionComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartFrictionComponent* FrictionComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartNetworkSyncComponent* NetworkSyncComponent = nullptr;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed = 4000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	float TotalMaxSpeed = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	float NormalizedSpeed = 0.0f;
#pragma endregion

#pragma region Niagara
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartSkidMarkComponent* LeftSkidMark = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartSkidMarkComponent* RightSkidMark = nullptr;
#pragma endregion
	
#pragma region UI
	void UpdateSpeedUI();
#pragma endregion
};
