// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "KartFrictionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartFrictionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartFrictionComponent();

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

	void ApplyFriction(float DeltaTime);
private:
	void OnDriftInputDetected(const FInputActionValue& InputActionValue);
	UFUNCTION(Server, Reliable)
	void ApplyFrictionToKart(bool bInDrift, float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Drift = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	bool bDrift = false;
};
