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

	if (GetLocalRole() == ROLE_Authority)
	{
		// 서버 시간을 계속해서 업데이트한다.
		ServerWorldTimeSeconds = GetWorld()->GetTimeSeconds();
	}
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

		if (PSA->GetIsRaceEnd())
		{
			return true;
		}

		if (PSA->GetIsRaceEnd() && PSB->GetIsRaceEnd())
		{
			return PSA->GetRaceEndTime() < PSB->GetRaceEndTime();
		}
		
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

#pragma region NetworkClock
double ARaceGameState::GetServerWorldTimeSeconds() const
{
	// 기존의 시간을 가져오는 함수를 사용하지 않고, 서버 시간을 가져오도록 변경한다.
	// return Super::GetServerWorldTimeSeconds();

	// 서버 자기 자신이 요청하는 경우 서버 시간을 반환한다.
	if (GetLocalRole() == ROLE_Authority)
	{
		return ServerWorldTimeSeconds;
	}

	// 클라이언트가 요청하는 경우 클라이언트가 서버와 직접 동기화된 시간을 가져온다.
	// FirstPlayerController를 가져오는 이유는 해당 플레이어 컨트롤러에서만 관리되기 때문이다.
	if(ARacePlayerController* PC = Cast<ARacePlayerController>(GetGameInstance()->GetFirstLocalPlayerController(GetWorld())))
	{
		return PC->GetServerTime();
	}
	else
	{
		return GetWorld()->GetTimeSeconds();
	}
}
#pragma endregion

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

void ARaceGameState::SetReadyForTheMatch()
{
	if (!StartPoint)
	{
		UE_LOG(LogTemp, Error, TEXT("제대로 활성화 되지 않은 트랙입니다. 개발 상태로 전환됩니다."))
		return;
	}
	
	SetRaceStatus(ERaceStatus::Ready);
	
	for (int i = 0; i < PlayerArray.Num(); i++)
	{
		ARacePlayerController* PC = Cast<ARacePlayerController>(
				PlayerArray[i]->GetPlayerController());
		
		if (!PC)
		{
			return;
		}

		if (PC->IsLocalController())
		{
			PC->SetHUDToStart();
		} else
		{
			PC->Client_SetHUDToStart();
		}
	}
}