#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KartCollisionComponentLegacy.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTR_API UKartCollisionComponentLegacy : public UActorComponent
{
	GENERATED_BODY()

public:
	UKartCollisionComponentLegacy();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 BaseCollisionCooldownFrame = 10.f;
	
	UPROPERTY(Replicated)
	uint8 CurrentCollisionCooldownFrame;
	
	UPROPERTY()
	float CollisionPower = 0.6;
	
	UPROPERTY()
	class AKart* Kart = nullptr;

	UFUNCTION()
	void OnCollisionKart(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);
};
