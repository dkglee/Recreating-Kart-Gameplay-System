#include "CheckPoint.h"

#include "RaceGameState.h"
#include "Components/BoxComponent.h"
#include "ProjectR/Player/RiderPlayerState.h"

ACheckPoint::ACheckPoint()
{
	CheckBox = CreateDefaultSubobject<UBoxComponent>("Check Box");
	CheckBox->SetupAttachment(GetRootComponent());
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	GetCheckPointPinInfo(CurrentCheckPoint, CurrentCheckPointPinNumArray);
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
	const APawn* PlayerKart = Cast<APawn>(CheckKart);
	if (!PlayerKart)
	{
		return;
	}

	ARiderPlayerState* RiderPlayerState = PlayerKart->GetPlayerState<ARiderPlayerState>();

	if (!RiderPlayerState)
	{
		return;
	}

	const FString CheckPointPin = RiderPlayerState->GetCurrentKartCheckPoint();

	TArray<uint16> CurrentPlayerPin;
	
	GetCheckPointPinInfo(CheckPointPin, CurrentPlayerPin);

	// 두개의 길이가 같은 경우는 계속 숫자들을 순회하면서 마지막 번호가 달라졌는지 확인해야 한다.
	// 갯수가 같은 경우는 무조건 분기점 추가가 아닌 다음 핀 포인트로 이동했음을 의미한다.
	// 여기서 4-2 -> 5-1 의 케이스도 있을 수 있으나, 해당 케이스는 고려하지 않는다.
	// 반드시 분기점 이전에 체크포인트를 설정하는 것을 가정하고 작업한다.
	// TODO: 해당 부분은 이후 툴 제작에도 영향을 줘야 할 필요가 있다.
	if (CurrentPlayerPin.Num() == CurrentCheckPointPinNumArray.Num())
	{
		// 단순하게 다음 핀으로 넘어가는 과정이라면
		// 아래와 같이 현재 체크포인트의 핀으로 설정해주면 된다.
		const bool IsNextPinToMove = CurrentPlayerPin[CurrentPlayerPin.Num() - 1] + 1 == CurrentCheckPointPinNumArray[CurrentCheckPointPinNumArray.Num() - 1];
		
		if (IsNextPinToMove)
		{
			RiderPlayerState->SetCheckPoint(CurrentCheckPoint);
			return;
		}

		// 위에 과정 즉 다음 핀으로 넘어가는 것이 아닌 바퀴 수가 넘어가는 조건 즉
		// 모든 체크포인트를 순회했으며, 다음 핀이 0인 경우는 무조건 한 바퀴를 돌았다고 가정한다.
		const bool IsNextLapToMove = CurrentPlayerPin[0] == GetWorld()->GetGameState<ARaceGameState>()->GetMaxCheckPoint()
									&& CurrentCheckPointPinNumArray[0] == 0;

		if (IsNextLapToMove)
		{
			RiderPlayerState->GoNextLap();
		}
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

void ACheckPoint::GetCheckPointPinInfo(FString Pin, TArray<uint16>& PinList)
{
	FString Temp;
	// wchar_t -> 확장형 문자 타입으로 OS 별 모든 것을 지원하는
	// FString을 구성하는 문자 요소인 것으로 추정
	for (const wchar_t Char : Pin)
	{
		// 글자가 분기점 : 인 경우에 대한 대응
		if (Char == ':')
		{
			PinList.Add(FCString::Atoi(*Temp));
			Temp = TEXT("");
			continue;
		}
		Temp.InsertAt(Temp.Len() - 1, Char);
	}
	
	PinList.Add(FCString::Atoi(*Temp));
}
