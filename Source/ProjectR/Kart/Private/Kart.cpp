#include "Kart.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "KartMovementComponent.h"
#include "KartSteeringComponent.h"

// Sets default values
AKart::AKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_KART
	(TEXT("/Game/Kart/Input/IMC_Kart.IMC_Kart"));
	if (IMC_KART.Succeeded())
	{
		Imc_Kart = IMC_KART.Object;
	}

	KartSteeringComponent = CreateDefaultSubobject<UKartSteeringComponent>(TEXT("KartSteeringComponent"));
	KartMovementComponent = CreateDefaultSubobject<UKartMovementComponent>(TEXT("KartMovementComponent"));
}

// Called when the game starts or when spawned
void AKart::BeginPlay()
{
	Super::BeginPlay();

	// Kart Input Binding
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(Imc_Kart, 0);
		}
	}
}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AKart::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* PlayerInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (PlayerInput)
	{
		OnInputBindingDelegate.Broadcast(PlayerInput);
	}
}

