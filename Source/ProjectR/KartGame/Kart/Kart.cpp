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
#include "CommonUtil.h"
#include "KartGame/Items/Components/ItemInteractionComponent.h"
#include "FastLogger.h"
#include "KartFrictionComponent.h"
#include "KartNetworkSyncComponent.h"
#include "KartGame/Games/Modes/RacePlayerController.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/HUD/DashBoard/DashBoardUI.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AKart::AKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	Super::SetReplicateMovement(false);

	SetNetUpdateFrequency(1000.0f);

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

	RootBox->SetLinearDamping(0.9f);
	RootBox->SetAngularDamping(0.9f);

	// 클라이언트에서 물리가 실행되게끔 해줄 수 있음.
	// 그렇다면 클라에서 실행된 물리가 서버로 동기화가 되어야 함.
	RootBox->bReplicatePhysicsToAutonomousProxy = false;

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
	LF_Wheel->SetNetAddressable();
	LF_Wheel->SetIsReplicated(true);
	
	RF_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("RF_Wheel"));
	RF_Wheel->SetupAttachment(RootBox);
	RF_Wheel->SetRelativeLocation({100, 50, 0});
	RF_Wheel->SetNetAddressable();
	RF_Wheel->SetIsReplicated(true);
	
	LR_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("LR_Wheel"));
	LR_Wheel->SetupAttachment(RootBox);
	LR_Wheel->SetRelativeLocation({-100, -50, 0});
	LR_Wheel->SetNetAddressable();
	LR_Wheel->SetIsReplicated(true);
	
	RR_Wheel = CreateDefaultSubobject<UKartSuspensionComponent>(TEXT("RR_Wheel"));
	RR_Wheel->SetupAttachment(RootBox);
	RR_Wheel->SetRelativeLocation({-100, 50, 0});
	RR_Wheel->SetNetAddressable();
	RR_Wheel->SetIsReplicated(true);

	AccelerationComponent = CreateDefaultSubobject<UKartAccelerationComponent>(TEXT("AccelerationComponent"));
	AccelerationComponent->SetNetAddressable();
	AccelerationComponent->SetIsReplicated(true);

	ItemInventoryComponent = CreateDefaultSubobject<UItemInventoryComponent>(TEXT("ItemInventoryComponent"));
	ItemInteractionComponent = CreateDefaultSubobject<UItemInteractionComponent>(TEXT("ItemInteractionComponent"));
	
	SteeringComponent = CreateDefaultSubobject<UKartSteeringComponent>(TEXT("SteeringComponent"));
	SteeringComponent->SetNetAddressable();
	SteeringComponent->SetIsReplicated(true);

	FrictionComponent = CreateDefaultSubobject<UKartFrictionComponent>(TEXT("FrictionComponent"));
	FrictionComponent->SetNetAddressable();
	FrictionComponent->SetIsReplicated(true);

	NetworkSyncComponent = CreateDefaultSubobject<UKartNetworkSyncComponent>(TEXT("NetworkSyncComponent"));
	NetworkSyncComponent->SetNetAddressable();
	NetworkSyncComponent->SetIsReplicated(true);
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

	if (IsLocallyControlled())
	{
		RootBox->SetSimulatePhysics(true);
	}
	else
	{
		RootBox->SetSimulatePhysics(false);
	}
}

void AKart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 로컬의 위치만 업데이트 됨
	if(ItemInteractionComponent->bIsInteraction == false && IsLocallyControlled())
	{
		CalcuateNormalizedSpeed();
		
		bool flag = true;
		
		flag &= LR_Wheel->ProcessSuspension();
		flag &= RR_Wheel->ProcessSuspension();
		flag &= LF_Wheel->ProcessSuspension();
		flag &= RF_Wheel->ProcessSuspension();
		
		if (flag)
		{
			SteeringComponent->ProcessSteeringAndTorque();
			AccelerationComponent->ProcessAcceleration();
			FrictionComponent->ProcessFriction();
		}
	}
	UpdateSpeedUI();
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

void AKart::CalcuateNormalizedSpeed()
{
	FVector ForwardVector = RootBox->GetForwardVector();
	FVector LinearVelocity = RootBox->GetPhysicsLinearVelocity();
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);
	NormalizedSpeed = FMath::Abs(KartSpeed) / MaxSpeed;
}

void AKart::UpdateSpeedUI()
{
	FVector LinearVelocity = RootBox->GetPhysicsLinearVelocity();
	FVector ForwardVector = RootBox->GetForwardVector();
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);

	ARacePlayerController* PC = Cast<ARacePlayerController>(GetController());
	if (PC)
	{
		UMainUI* MainUI = PC->GetMainHUD();
		if (!MainUI)
		{
			return ;
		}
		UDashBoardUI* DashBoardUI = MainUI->GetWBP_DashBoardUI();
		if (!DashBoardUI)
		{
			return ;
		}
		DashBoardUI->SetDashBoardValue(KartSpeed, TotalMaxSpeed);
	}
}
