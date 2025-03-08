#include "CheckPoint.h"

#include "Components/BoxComponent.h"


ACheckPoint::ACheckPoint()
{
	CheckBox = CreateDefaultSubobject<UBoxComponent>("Check Box");
	CheckBox->SetupAttachment(GetRootComponent());
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMoveToCheckPoint);
}

void ACheckPoint::OnMoveToCheckPoint(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	, bool bFromSweep, const FHitResult& SweepResult)
{
}
