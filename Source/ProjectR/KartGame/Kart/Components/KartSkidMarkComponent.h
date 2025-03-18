// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Components/SceneComponent.h"
#include "KartSkidMarkComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartSkidMarkComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartSkidMarkComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void ProcessSkidMark(bool bIsSkidding);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skid Mark", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;

	UFUNCTION(Server, Reliable)
	void Server_SetIsSkidActive(bool bIsSkidding);
	
	// 서버에서 업데이트
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_bIsSkidActive, BlueprintReadWrite, Category = "Skid Mark", meta = (AllowPrivateAccess = "true"))
	bool bIsSkidActive = false;
	UFUNCTION()
	void OnRep_bIsSkidActive();
};
