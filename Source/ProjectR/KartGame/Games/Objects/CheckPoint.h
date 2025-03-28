#pragma once

#include "CoreMinimal.h"
#include "CommonUtil.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTR_API ACheckPoint : public AActor
{
	GENERATED_BODY()

public:
	ACheckPoint();

	FORCEINLINE FString GetCurrentCheckPoint() const { return CurrentCheckPoint; }
	FORCEINLINE FString GetNextCheckPoint() const { return NextCheckPoint; }

	uint16 GetPinMainNumber() const;
	GETTER(uint8, MaxLaps)
	GETTER(bool, IsStart)

	FTransform GetTransformByStartIndex(const uint8 Index);
	
protected:
	virtual void BeginPlay() override;
	
private:
	TArray<uint16> CurrentCheckPointPinNumArray;
	
	UPROPERTY(EditAnywhere, Category = "Options|CheckPoint", meta = (AllowPrivateAccess = true))
	FString CurrentCheckPoint;
	
	UPROPERTY(EditAnywhere, Category = "Options|CheckPoint", meta = (AllowPrivateAccess = true))
	FString NextCheckPoint;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> CheckBox;

	UPROPERTY(EditAnywhere, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 IsStart:1;
	
	UPROPERTY(EditAnywhere, Category = "Options|Track", meta = (AllowPrivateAccess = true, EditCondition = IsStart))
	uint8 MaxLaps = 3;

	UFUNCTION()
	void OnMoveToCheckPoint(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void SaveCheckPoint(const AActor* CheckKart);

	UPROPERTY(EditAnywhere, Category = "Options|Track", meta = (AllowPrivateAccess = true, EditCondition = IsStart, MakeEditWidget = true))
	TArray<FVector> StartRelativePos;
};
