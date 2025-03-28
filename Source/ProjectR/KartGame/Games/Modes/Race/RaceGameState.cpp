#include "RaceGameState.h"

#include "RacePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectR/KartGame/Games/Objects/CheckPoint.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"
#include "Net/UnrealNetwork.h"

ARaceGameState::ARaceGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARaceGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARaceGameState, RaceStatus);
	DOREPLIFETIME(ARaceGameState, RaceStartTime);
	DOREPLIFETIME(ARaceGameState, RaceEndTime);
}

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

		if (CheckPointA->GetPinMainNumber() == CheckPointB->GetPinMainNumber() && CheckPointA->GetPinMainNumber() == 0)
		{
			return !CheckPointA->GetIsStart();
		}
		
		return CheckPointA->GetPinMainNumber() < CheckPointB->GetPinMainNumber();
	});

	// ReadOnly용 체크 포인트 데이터 가져오기
	for (AActor* CheckPoint : CheckPointList)
	{
		ACheckPoint* NewCheckPoint = static_cast<ACheckPoint*>(CheckPoint);
		CheckPointData.Add(NewCheckPoint->GetCurrentCheckPoint(), NewCheckPoint);
	}

	StartPoint = static_cast<ACheckPoint*>(CheckPointList[0]);
	const ACheckPoint* LastPoint = static_cast<ACheckPoint*>(CheckPointList[CheckPointList.Num() - 1]);
	// 체크포인트 최대 숫자는 보통 0 ~ 최대 숫자 까지기 때문에 0 ~ 최대 숫자 만큼 보다
	// 적은 갯수를 보유하고 있으면 맵에 있는 체크포인트 자체에 문제가 있음을 의미한다.
	if (CheckPointList.Num() < LastPoint->GetPinMainNumber() + 1)
	{
		UE_LOG(LogTemp, Error, TEXT("현재 잘못된 맵 세팅입니다. 재확인이 필요합니다: 사유 체크포인트 갯수 부족"))
	}

	MaxLaps = StartPoint->GetMaxLaps();
	MaxCheckPoint = LastPoint->GetPinMainNumber();
}

void ARaceGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SortRank();
}

void ARaceGameState::SortRank()
{
	if (!HasAuthority())
	{
		return;
	}

	if (RaceStatus == ERaceStatus::Idle)
	{
		return;
	}
	
	if (RaceStatus == ERaceStatus::Finish)
	{
		return;
	}
	
	Algo::HeapSort(PlayerArray, [](const APlayerState* A, const APlayerState* B)
	{
		if (!IsValid(A) || !IsValid(B))
		{
			return true;
		}
		
		const ARiderPlayerState* PSA = Cast<ARiderPlayerState>(A);
		const ARiderPlayerState* PSB = Cast<ARiderPlayerState>(B);

		// 서로 이동한 랩 정보가 다르다면, 랩을 더 많이 돌은 사람이 우선순위가 높다.
		if (PSA->GetCurrentLap() != PSB->GetCurrentLap())
		{
			return PSA->GetCurrentLap() > PSB->GetCurrentLap();
		}

		// 체크포인트 끼리 닿은 경우에 대해서는 서브 체크포인트가 아닌
		// 메인 체크포인트 번호를 비교해서 처리한다.
		if (PSA->GetCurrentKartCheckPoint() != PSB->GetCurrentKartCheckPoint())
		{
			return PSA->GetCurrentMainCheckPoint() > PSB->GetCurrentMainCheckPoint();
		}
		
		// 여기서부터는 수식이 들어간다. 가장 가까운 다음 체크포인트 정보를 비교하는 방식으로 처리한다.
		if (!PSA->GetNextNearCheckPoint())
		{
			return true;
		}
		
		if (!PSB->GetNextNearCheckPoint())
		{
			return true;
		}
		
		const float DistanceA = (PSA->GetNextNearCheckPoint()->GetActorLocation() - PSA->GetPawn()->GetActorLocation()).Length();
		const float DistanceB = (PSB->GetNextNearCheckPoint()->GetActorLocation() - PSB->GetPawn()->GetActorLocation()).Length();
		
		return DistanceA < DistanceB;
	});

	int Rank = 0;
	for (const auto PlayerState : PlayerArray)
	{
		Rank += 1;
		ARiderPlayerState* PS = Cast<ARiderPlayerState>(PlayerState);
		PS->SetRanking(Rank);
	}
}

void ARaceGameState::CountDownToFinish(const FDateTime& FinishTime)
{
	if (!HasAuthority())
	{
		return;
	}

	if (GetRaceStatus() != ERaceStatus::Playing)
	{
		return;
	}
	
	SetRaceStatus(ERaceStatus::HoldToFinish);
	RaceEndTime = FinishTime;

	for (TObjectPtr<APlayerState> PlayerState : PlayerArray)
	{
		ARacePlayerController* PC = Cast<ARacePlayerController>(PlayerState->GetPlayerController());
		if (!PC)
		{
			continue;
		}

		if (PC->IsLocalController())
		{
			PC->CountDownToEndGame();
		} else
		{
			PC->Client_CountDownToEndGame();
		}
	}
}