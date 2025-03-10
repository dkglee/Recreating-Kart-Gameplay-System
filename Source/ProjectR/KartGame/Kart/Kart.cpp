#include "Kart.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Chaos/SoftsSpring.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kart/Suspension/Public/KartSuspensionComponent.h"

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

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(RootBox);
	RootBox->SetBoxExtent({100, 60, 32});

	RootBox->SetSimulatePhysics(true);
	RootBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootBox->SetCollisionResponseToAllChannels(ECR_Block);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootBox);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->SocketOffset = {0, 0, 100.0f};
	SpringArmComponent->bUsePawnControlRotation = true;

	LF_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("LF_Wheel"));
	LF_Wheel->SetupAttachment(RootBox);
	LF_Wheel->SetRelativeLocation({80, -70, -20});
	RF_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("RF_Wheel"));
	RF_Wheel->SetupAttachment(RootBox);
	RF_Wheel->SetRelativeLocation({80, 70, -20});
	LR_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("LR_Wheel"));
	LR_Wheel->SetupAttachment(RootBox);
	LR_Wheel->SetRelativeLocation({-90, -70, -20});
	RR_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("RR_Wheel"));
	RR_Wheel->SetupAttachment(RootBox);
	RR_Wheel->SetRelativeLocation({-90, 70, -20});
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

