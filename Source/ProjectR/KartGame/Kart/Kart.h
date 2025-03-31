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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region GetterSetter
	GETTER(class UItemInventoryComponent*, ItemInventoryComponent);
	GETTER(class UItemInteractionComponent*, ItemInteractionComponent);
	GETTER(class UKartSteeringComponent*, SteeringComponent);
	GETTER(float, NormalizedSpeed);
	GETTER(class UKartAccelerationComponent*, AccelerationComponent);
	GETTER(class UKartFrictionComponent*, FrictionComponent);
	GETTER(class UKartCollisionComponent*, KartCollisionComponent);
	GETTER(class UBoxComponent*, RootBox);
	GETTER(class UKartSuspensionComponent*, LF_Wheel);
	GETTER(class UKartSuspensionComponent*, RF_Wheel);
	GETTER(class UKartSuspensionComponent*, LR_Wheel);
	GETTER(class UKartSuspensionComponent*, RR_Wheel);
	GETTER(class UKartNetworkSyncComponent*, NetworkSyncComponent);
	GETTER(float, MaxSpeed);
	GETTER_SETTER(bool, bCanMove);
	GETTER(class USceneComponent*, TargetAimSocketComponent);
	GETTER(class UWidgetComponent*, UsingAimComponent);
	GETTER(class UKartBoosterComponent*, BoosterComponent);
	GETTER(class UKartDraftComponent*, DraftComponent);
	GETTER_SETTER(bool, bUsingBooster);
	GETTER(class USpringArmComponent*, SpringArmComponent);
	GETTER(class UCameraComponent*, CameraComponent);
	GETTER(class UBoosterGaugeUI*, BoosterGaugeUI);
	GETTER(class UKartShieldVFXComponent*, Shield);
#pragma endregion

#pragma region ForeignFunction
	UFUNCTION()
	void ClearAcceleration();
#pragma endregion
	
	FInputBindingDelegate OnInputBindingDelegate;

private:
	
#pragma region ForeignVariable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	bool bCanMove = true;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	bool bUsingBooster = false;
#pragma endregion

#pragma region CoreSkills
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
	class UKartCollisionComponent* KartCollisionComponent = nullptr;
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartResetComponent* KartResetComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartBoosterComponent* BoosterComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartDraftComponent* DraftComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	class UKartSkeletalMeshComponent* KartSkeletalMeshComponent = nullptr;
	
	// Aim Widget Component 추가
	// 장진혁
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AimUI", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* TargetAimSocketComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AimUI", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* UsingAimComponent = nullptr;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	float MaxSpeed = 2000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	float TotalMaxSpeed = 5000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart", meta = (AllowPrivateAccess = "true"))
	float NormalizedSpeed = 0.0f;
#pragma endregion

#pragma region Niagara
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartSkidMarkComponent* LeftSkidMark = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartSkidMarkComponent* RightSkidMark = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartBasicBoosterVFXComponent* LeftInstantBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartBasicBoosterVFXComponent* RightInstantBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartBasicBoosterVFXComponent* LeftBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartBasicBoosterVFXComponent* RightBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartBasicBoosterVFXComponent* AirBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartShieldVFXComponent* Shield;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartSparkleVFXComponent* LeftSparkleVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara", meta = (AllowPrivateAccess = "true"))
	class UKartSparkleVFXComponent* RightSparkleVFX;
#pragma endregion
	
#pragma region UI
	void UpdateSpeedUI();
#pragma endregion

#pragma region Sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	class UKartEngineSoundComponent* EngineSoundComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	class UKartDriftSoundComponent* DriftSoundComponent = nullptr;
#pragma endregion

#pragma region Etc
	TArray<FVector> LineTraceLocations;
#pragma endregion

#pragma region Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UKartCameraComponent* KartCameraComponent = nullptr;
#pragma endregion

#pragma region UI
	UPROPERTY()
	class USpeedLineUI* SpeedLineUI = nullptr;
	UPROPERTY()
	TSubclassOf<class USpeedLineUI> SpeedLineUIClass;
	UPROPERTY()
	class UBoosterGaugeUI* BoosterGaugeUI = nullptr;
	UPROPERTY()
	TSubclassOf<class UBoosterGaugeUI> BoosterGaugeUIClass;
#pragma endregion
};
