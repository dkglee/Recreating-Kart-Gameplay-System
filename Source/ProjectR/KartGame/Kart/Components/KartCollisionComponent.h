#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartCollisionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UKartCollisionComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(Replicated)
	uint8 CollisionCooldownFrame;
	
	UPROPERTY()
	class AKart* Kart = nullptr;

	UFUNCTION()
	void OnCollisionKart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
