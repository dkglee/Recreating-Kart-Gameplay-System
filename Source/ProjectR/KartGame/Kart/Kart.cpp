#include "Kart.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "KartAccelerationComponent.h"
#include "KartSteeringComponent.h"
#include "Camera/CameraComponent.h"
#include "Chaos/SoftsSpring.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "KartSuspensionComponent.h"
#include "KartGame/Items/Components/ItemInventoryComponent.h"

// Sets default values
AKart::AKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	Super::SetReplicateMovement(true);

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_KART
	(TEXT("/Game/Kart/Input/IMC_Kart.IMC_Kart"));
	if (IMC_KART.Succeeded())
	{
		Imc_Kart = IMC_KART.Object;
	}

	RootBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(RootBox);
	RootBox->SetBoxExtent({103, 52, 12});

	RootBox->SetSimulatePhysics(true);
	RootBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootBox->SetCollisionResponseToAllChannels(ECR_Block);

	RootBox->SetLinearDamping(3.0f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootBox);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->SocketOffset = {0, 0, 100.0f};
	SpringArmComponent->bUsePawnControlRotation = false;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bEnableCameraRotationLag = true;

	LF_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("LF_Wheel"));
	LF_Wheel->SetupAttachment(RootBox);
	LF_Wheel->SetRelativeLocation({100, -50, 0});
	RF_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("RF_Wheel"));
	RF_Wheel->SetupAttachment(RootBox);
	RF_Wheel->SetRelativeLocation({100, 50, 0});
	LR_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("LR_Wheel"));
	LR_Wheel->SetupAttachment(RootBox);
	LR_Wheel->SetRelativeLocation({-100, -50, 0});
	RR_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("RR_Wheel"));
	RR_Wheel->SetupAttachment(RootBox);
	RR_Wheel->SetRelativeLocation({-100, 50, 0});

	AccelerationComponent = CreateDefaultSubobject<UKartAccelerationComponent>(TEXT("AccelerationComponent"));
	ItemInventoryComponent = CreateDefaultSubobject<UItemInventoryComponent>(TEXT("ItemInventoryComponent"));
	SteeringComponent = CreateDefaultSubobject<UKartSteeringComponent>(TEXT("SteeringComponent"));
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

	LR_Wheel->ProcessSuspension();
	RR_Wheel->ProcessSuspension();
	LF_Wheel->ProcessSuspension();
	RF_Wheel->ProcessSuspension();
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

class UItemInventoryComponent* AKart::GetItemInventoryComponent()
{
	return ItemInventoryComponent;
}
