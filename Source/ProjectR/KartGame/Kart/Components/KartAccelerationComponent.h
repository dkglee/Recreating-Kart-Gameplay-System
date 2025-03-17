// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "KartAccelerationComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartAccelerationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartAccelerationComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	UFUNCTION()
	void SetupInputBinding(class UEnhancedInputComponent* PlayerInputComponent);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void ApplyAcceleration(float DeltaTime);

private:
	void OnMovementInputDetected(const FInputActionValue& InputActionValue);
	void ProcessAcceleration(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ApplyForceToKart(float InAcceleration, float DeltaTime);

	// InitializeComponent에서 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	TArray<class UKartSuspensionComponent*> Wheels;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Movement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	float AccelerationInput = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	float AccelerationRate = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	float DragCoefficient = 0.3f;

	// 엔진에 따라서 변경될 수 있음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	float MaxAcceleration = 3000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Movement", meta = (AllowPrivateAccess = "true"))
	float Acceleration = 0.0f;
};
