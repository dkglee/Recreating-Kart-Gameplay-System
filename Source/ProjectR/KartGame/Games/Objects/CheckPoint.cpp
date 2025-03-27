#include "CheckPoint.h"

#include "Kart.h"
#include "Components/BoxComponent.h"
#include "ProjectR/KartGame/Utils/CheckPointUtil.h"
#include "KartGame/Games/Modes/Race/RaceGameState.h"
#include "KartGame/Games/Modes/Race/RiderPlayerState.h"

ACheckPoint::ACheckPoint()
{
	CheckBox = CreateDefaultSubobject<UBoxComponent>("Check Box");
	CheckBox->SetupAttachment(GetRootComponent());
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	FCheckPointUtil::GetCheckPointPinInfo(CurrentCheckPoint, CurrentCheckPointPinNumArray);
	CheckBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnMoveToCheckPoint);
}

void ACheckPoint::OnMoveToCheckPoint(UPrimitiveComponent* OverlappedComponent
	, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	, bool bFromSweep, const FHitResult& SweepResult)
{
	SaveCheckPoint(OtherActor);
}

void ACheckPoint::SaveCheckPoint(const AActor* CheckKart)
{
	if (!HasAuthority())
	{
		return;
	}
	
	const AKart* PlayerKart = Cast<AKart>(CheckKart);
	if (!PlayerKart)
	{
		return;
	}

	ARiderPlayerState* PS = PlayerKart->GetPlayerState<ARiderPlayerState>();

	if (!PS)
	{
		return;
	}

	const FString CheckPointPin = PS->GetCurrentKartCheckPoint();

	TArray<uint16> CurrentPlayerPin;
	
	FCheckPointUtil::GetCheckPointPinInfo(CheckPointPin, CurrentPlayerPin);

	const uint16 CurrentPlayerPinMainNum = CurrentPlayerPin[0];
	const uint16 CurrentCheckPointPinMainNum = CurrentCheckPointPinNumArray[0];
	
	// 두개의 길이가 같은 경우는 계속 숫자들을 순회하면서 마지막 번호가 달라졌는지 확인해야 한다.
	// 갯수가 같은 경우는 무조건 분기점 추가가 아닌 다음 핀 포인트로 이동했음을 의미한다.
	// 핀 포인트 분기 이동 방식은 4-2 -> 5-2 같은 항상 체크포인트는 메인 핀 포인트를 기점으로
	// 움직이게 끔 처리된다.
	// TODO: 해당 부분은 이후 툴 제작에도 영향을 줘야 할 필요가 있다.
	const bool IsNextPinToMove = CurrentPlayerPinMainNum + 1 == CurrentCheckPointPinMainNum;

	UE_LOG(LogTemp, Display, TEXT("현재 정보: %d Laps, %s CheckPoint"), PS->GetCurrentLap(), *PS->GetCurrentKartCheckPoint());
	if (IsNextPinToMove)
	{
		PS->SetCheckPoint(CurrentCheckPoint);
		UE_LOG(LogTemp, Display, TEXT("다음 체크포인트 이동"));
		return;
	}

	// 위에 과정 즉 다음 핀으로 넘어가는 것이 아닌 바퀴 수가 넘어가는 조건 즉
	// 모든 체크포인트를 순회했으며, 다음 핀이 0인 경우는 무조건 한 바퀴를 돌았다고 가정한다.
	const bool IsNextLapToMove = CurrentPlayerPinMainNum == GetWorld()->GetGameState<ARaceGameState>()->GetMaxCheckPoint()
								&& GetPinMainNumber() == 0;

	if (IsNextLapToMove)
	{
		UE_LOG(LogTemp, Display, TEXT("다음 랩 이동"));
		PS->GoNextLap();
	}
}

uint16 ACheckPoint::GetPinMainNumber() const
{
	// 실제로 서비스 시 이런 일은 없겠지만 해당 부분은 미리 에디터 환경에서
	// 알 수 있도록 처리해두면 더 좋을 것 같다.
	if (CurrentCheckPointPinNumArray.IsEmpty())
	{
		return -1;
	}
	
	return CurrentCheckPointPinNumArray[0];
}

FTransform ACheckPoint::GetTransformByStartIndex(const uint8 Index)
{
	FTransform NewTransform;
	NewTransform.SetLocation(GetActorLocation() + StartRelativePos[Index]);
	NewTransform.SetRotation(FQuat::MakeFromRotator(GetActorRotation()));

	return NewTransform;
}
