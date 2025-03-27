// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "KartInfo.h"
#include "Components/ActorComponent.h"
#include "KartNetworkSyncComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartNetworkSyncComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKartNetworkSyncComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

#pragma region GetterSetters
	GETTER(FKartInfo, KartInfo);
#pragma endregion

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class AKart* Kart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kart Steering", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* KartBody = nullptr;

	// 서버에서 클라이언트로 위치와 회전값을 Replicate하기 위한 변수
	// 자기 자신(Local Controlled)인 경우 해당 값을 무시
	// Simulated Proxy인 경우 해당 값을 적용
	UPROPERTY(ReplicatedUsing=OnRep_KartInfo)
	FKartInfo KartInfo;
	
	UFUNCTION()
	void OnRep_KartInfo();

	UFUNCTION(Server, Reliable)
	void Server_SendKartInfo(FKartInfo NewKartInfo);
};
