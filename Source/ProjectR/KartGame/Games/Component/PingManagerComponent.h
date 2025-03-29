#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PingManagerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPingAccessNotified);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLocalPingAccessNotified);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UPingManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPingManagerComponent();

	FOnAllPingAccessNotified OnAllPingAccessNotified;
	FOnLocalPingAccessNotified OnLocalPingAccessNotified;
	
protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void GetLifetimeReplicatedProps(
		TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	bool IsPingReceived = false;

	void PingToServer();
	
	UFUNCTION(Server, Reliable)
	void Server_PingToServer();
};
