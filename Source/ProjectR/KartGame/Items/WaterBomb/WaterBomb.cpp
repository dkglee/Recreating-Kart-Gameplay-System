// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBomb.h"

#include "FastLogger.h"
#include "Kart.h"
#include "Components/BoxComponent.h"
#include "KartGame/Items/Components/ItemInteractionComponent.h"


// Sets default values
AWaterBomb::AWaterBomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Super::SetReplicateMovement(true);
	Root->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AWaterBomb::BeginPlay()
{
	Super::BeginPlay();
	
	Root->OnComponentBeginOverlap.AddDynamic(this, &AWaterBomb::OnWaterBombBeginOverlap);
}

// Called every frame
void AWaterBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bArriveEndPos == false)
	{
		MoveToEstimateLocation(DeltaTime);
	}
	else
	{
		SetWaterBombScale(DeltaTime);
	}
}

void AWaterBomb::OnWaterBombBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false) return;
	
	auto* Kart = Cast<AKart>(OtherActor);
	if (Kart == GetOwningPlayer())
	{
		FFastLogger::LogConsole(TEXT("Hit Kart is itemOwner"));
		return;
	}
	
	if (Kart)
	{
		// 이미 물풍선에 맞았으면 return;
		if (Kart->GetItemInteractionComponent()->GetbIsInteraction() == true &&
			Kart->GetItemInteractionComponent()->GetCurrentType() == EInteractionType::Water)
		{
			FFastLogger::LogConsole(TEXT("This actor is already interact"));
			return;
		}
		Kart->GetItemInteractionComponent()->Interaction(EInteractionType::Water);
	}
}


void AWaterBomb::MoveToEstimateLocation(float DeltaTime)
{
	if (HasAuthority() == false) return;
	
	if (StartPos == FVector::ZeroVector && EndPos == FVector::ZeroVector)
	{
		StartPos = GetOwningPlayer()->GetActorLocation();
		EndPos = StartPos + GetOwningPlayer()->GetActorForwardVector() * ThrowingDistance;
	}
	
	MoveElapsedTime += DeltaTime;
	float alpha = FMath::Clamp(MoveElapsedTime / MovementDuration, 0.0f, 1.0f);

	FVector LinearPos = FMath::Lerp(StartPos, EndPos, alpha);

	float ParabolaHeight = Height * (4 * alpha * (1 - alpha));
	LinearPos.Z += ParabolaHeight;

	if (alpha == 1.0f)
	{
		bArriveEndPos = true;
		return;
	}
	NetMulticast_MoveToEstimateLocation(LinearPos);	
}

void AWaterBomb::NetMulticast_MoveToEstimateLocation_Implementation(FVector resultPos)
{
	SetActorLocation(resultPos);
}

void AWaterBomb::SetWaterBombScale(float DeltaTime)
{
	if (HasAuthority() == false) return;
	
	if (InitialScale == FVector::ZeroVector)
	{
		InitialScale = GetOwningPlayer()->GetActorScale();
	}

	ScaleElapsedTime += DeltaTime;
	if (bIsScaleBigger == false)
	{
		float alpha = FMath::Clamp(ScaleElapsedTime / 0.1f, 0.0f, 1.0f);
		
		FVector LinearScale = FMath::Lerp(InitialScale, BigScale, alpha);

		if (ScaleElapsedTime >= ScaleDuration)
		{
			bIsScaleBigger = true;
			ScaleElapsedTime = 0.f;
		}
		NetMulticast_SetWaterBombScale(LinearScale);
	}
	else
	{
		float alpha = FMath::Clamp(ScaleElapsedTime / 0.1f, 0.0f, 1.0f);
		
		FVector LinearScale = FMath::Lerp(InitialScale, FVector::ZeroVector, alpha);

		if (GetActorScale().Size() <= InitialScale.Size())
		{
			FFastLogger::LogConsole(TEXT("물풍선 크기 작아져서 사라짐"));
			Destroy();
		}
		NetMulticast_SetWaterBombScale(LinearScale);
	}
}

void AWaterBomb::NetMulticast_SetWaterBombScale_Implementation(FVector resultScale)
{
	SetActorScale3D(resultScale);
}
