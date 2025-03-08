#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTR_API ACheckPoint : public AActor
{
	GENERATED_BODY()

public:
	ACheckPoint();

	FORCEINLINE uint16 GetCurrentCheckPoint() const { return CurrentCheckPoint; }
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Options|CheckPoint", meta = (AllowPrivateAccess = true))
	uint16 CurrentCheckPoint;
	
	UPROPERTY()
	TObjectPtr<UBoxComponent> CheckBox;

	UFUNCTION()
	void OnMoveToCheckPoint(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
