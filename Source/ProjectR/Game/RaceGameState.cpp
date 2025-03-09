#include "RaceGameState.h"

#include "CheckPoint.h"
#include "Kismet/GameplayStatics.h"

void ARaceGameState::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> CheckPointList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPoint::StaticClass(), CheckPointList);

	if (CheckPointList.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("현재 잘못된 맵 세팅입니다. 재확인이 필요합니다: 사유 체크포인트 존재하지 않음"))
		return;
	}
	
	Algo::HeapSort(CheckPointList, [](const AActor* A, const AActor* B)
	{
		const ACheckPoint* CheckPointA = static_cast<const ACheckPoint*>(A);
		const ACheckPoint* CheckPointB = static_cast<const ACheckPoint*>(B);
		
		return CheckPointA->GetCurrentCheckPoint() < CheckPointB->GetCurrentCheckPoint();
	});

	const ACheckPoint* LastPoint = static_cast<ACheckPoint*>(CheckPointList[CheckPointList.Num() - 1]);
	// 체크포인트 최대 숫자는 보통 0 ~ 최대 숫자 까지기 때문에 0 ~ 최대 숫자 만큼 보다
	// 적은 갯수를 보유하고 있으면 맵에 있는 체크포인트 자체에 문제가 있음을 의미한다.
	if (CheckPointList.Num() < LastPoint->GetCurrentCheckPoint() + 1)
	{
		UE_LOG(LogTemp, Error, TEXT("현재 잘못된 맵 세팅입니다. 재확인이 필요합니다: 사유 체크포인트 갯수 부족"))
	}
}
