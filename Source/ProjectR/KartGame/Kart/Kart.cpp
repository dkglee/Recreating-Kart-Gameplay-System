#include "Kart.h"

#include "BoosterGaugeUI.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "KartAccelerationComponent.h"
#include "KartAirBoostVFXComponent.h"
#include "KartBasicBoosterVFXComponent.h"
#include "KartBoosterComponent.h"
#include "KartCameraComponent.h"
#include "KartCollisionComponent.h"
#include "KartDraftComponent.h"
#include "KartDriftSoundComponent.h"
#include "KartEngineSoundComponent.h"
#include "KartSteeringComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "KartSuspensionComponent.h"
#include "KartGame/Items/Components/ItemInventoryComponent.h"
#include "KartGame/Items/Components/ItemInteractionComponent.h"
#include "KartFrictionComponent.h"
#include "KartInstantBoostVFXComponent.h"
#include "KartNetworkSyncComponent.h"
#include "KartResetComponent.h"
#include "KartSkeletalMeshComponent.h"
#include "KartGame/Games/Modes/Race/RacePlayerController.h"
#include "KartSkidMarkComponent.h"
#include "Components/WidgetComponent.h"
#include "KartSystemLibrary.h"
#include "KartGame/UIs/HUD/MainUI.h"
#include "KartGame/UIs/HUD/Aim/Aim.h"
#include "KartGame/UIs/HUD/DashBoard/DashBoardUI.h"
#include "Net/UnrealNetwork.h"
#include "KartPowerBoosterVFXComponent.h"
#include "KartShieldVFXComponent.h"
#include "SpeedLineUI.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
AKart::AKart()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	Super::SetReplicateMovement(false);

	SetNetUpdateFrequency(10000.0f);

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
	// Rootbox의 Simulation Generate Hit Event를 true로 해줘야 물리 충돌이 발생할 때 Hit Event가 발생함.
	RootBox->SetNotifyRigidBodyCollision(true);
	
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
	SpringArmComponent->bEnableCameraLag = false;
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->TargetArmLength = 450.0f;
	SpringArmComponent->SocketOffset = {0, 0, 150.0f};
	SpringArmComponent->bDoCollisionTest = false;
	CameraComponent->SetRelativeRotation({-10, 0, 0});
	
	KartCameraComponent = CreateDefaultSubobject<UKartCameraComponent>(TEXT("KartCameraComponent"));
	KartCameraComponent->SetNetAddressable();
	KartCameraComponent->SetIsReplicated(true);

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
	ItemInventoryComponent->SetNetAddressable();
	ItemInventoryComponent->SetIsReplicated(true);
	
	ItemInteractionComponent = CreateDefaultSubobject<UItemInteractionComponent>(TEXT("ItemInteractionComponent"));
	ItemInteractionComponent->SetNetAddressable();
	ItemInteractionComponent->SetIsReplicated(true);
	
	SteeringComponent = CreateDefaultSubobject<UKartSteeringComponent>(TEXT("SteeringComponent"));
	SteeringComponent->SetNetAddressable();
	SteeringComponent->SetIsReplicated(true);

	FrictionComponent = CreateDefaultSubobject<UKartFrictionComponent>(TEXT("FrictionComponent"));
	FrictionComponent->SetNetAddressable();
	FrictionComponent->SetIsReplicated(true);

	NetworkSyncComponent = CreateDefaultSubobject<UKartNetworkSyncComponent>(TEXT("NetworkSyncComponent"));
	NetworkSyncComponent->SetNetAddressable();
	NetworkSyncComponent->SetIsReplicated(true);

	LeftSkidMark = CreateDefaultSubobject<UKartSkidMarkComponent>(TEXT("LeftSkidMark"));
	LeftSkidMark->SetupAttachment(LR_Wheel);
	LeftSkidMark->SetNetAddressable();
	LeftSkidMark->SetIsReplicated(true);
	LeftSkidMark->SetRelativeLocation(FVector{25.000000, 10.000000, -5.000000});

	RightSkidMark = CreateDefaultSubobject<UKartSkidMarkComponent>(TEXT("RightSkidMark"));
	RightSkidMark->SetupAttachment(RR_Wheel);
	RightSkidMark->SetNetAddressable();
	RightSkidMark->SetIsReplicated(true);
	RightSkidMark->SetRelativeLocation(FVector{25.000000, -10.000000, -5.000000});

	EngineSoundComponent = CreateDefaultSubobject<UKartEngineSoundComponent>(TEXT("EngineSoundComponent"));
	EngineSoundComponent->SetupAttachment(RootBox);
	EngineSoundComponent->SetIsReplicated(false);

	DriftSoundComponent = CreateDefaultSubobject<UKartDriftSoundComponent>(TEXT("DriftSoundComponent"));
	DriftSoundComponent->SetupAttachment(RootBox);
	DriftSoundComponent->SetIsReplicated(false);

	// Aim UI Widget Component 추가
	// 장진혁
	TargetAimSocketComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TargetAimSocketComponent"));
	TargetAimSocketComponent->SetupAttachment(RootComponent);
	TargetAimSocketComponent->SetRelativeLocation(FVector(-50.f, 0.f, 50.f));

	UsingAimComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UsingAimComponent"));
	UsingAimComponent->SetupAttachment(RootComponent);
	UsingAimComponent->SetCastShadow(false);
	UsingAimComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	UsingAimComponent->SetVisibility(false);

	UsingAimComponent->SetRelativeLocation(FVector(150.f,  0.f,  100.f));
	UsingAimComponent->SetRelativeRotation(FRotator(0, 180, 0));
	UsingAimComponent->SetRelativeScale3D(FVector(0.5f));

	static ConstructorHelpers::FClassFinder<UAim> UsingAimUI(TEXT("'/Game/UIs/HUD/Aim/WBP_Aim.WBP_Aim_C'"));
	if (UsingAimUI.Succeeded())
	{
		UsingAimComponent->SetWidgetClass(UsingAimUI.Class);
	}
	
	KartResetComponent = CreateDefaultSubobject<UKartResetComponent>(TEXT("KartResetComponent"));
	KartResetComponent->SetNetAddressable();
	KartResetComponent->SetIsReplicated(true);
	
	KartCollisionComponent = CreateDefaultSubobject<UKartCollisionComponent>(TEXT("KartCollisionComponent"));
	KartCollisionComponent->SetNetAddressable();
	KartCollisionComponent->SetIsReplicated(true);

	BoosterComponent = CreateDefaultSubobject<UKartBoosterComponent>(TEXT("KartBoosterComponent"));
	BoosterComponent->SetNetAddressable();
	BoosterComponent->SetIsReplicated(true);

	
	DraftComponent = CreateDefaultSubobject<UKartDraftComponent>(TEXT("Kart Draft Component"));
	DraftComponent->SetNetAddressable();
	DraftComponent->SetIsReplicated(true);

	
	KartSkeletalMeshComponent = CreateDefaultSubobject<UKartSkeletalMeshComponent>(TEXT("KartSkeletalMeshComponent"));
	KartSkeletalMeshComponent->SetupAttachment(RootBox);
	KartSkeletalMeshComponent->SetNetAddressable();
	KartSkeletalMeshComponent->SetIsReplicated(true);

	// Linetrace Location 초기화
	LineTraceLocations.Reserve(4);
	LineTraceLocations.Add(FVector::ZeroVector);
	LineTraceLocations.Add(FVector::ZeroVector);
	LineTraceLocations.Add(FVector::ZeroVector);
	LineTraceLocations.Add(FVector::ZeroVector);

	// Booster Niagara
	LeftInstantBoost = CreateDefaultSubobject<UKartInstantBoostVFXComponent>(TEXT("InstBoostLeft"));
	LeftInstantBoost->SetupAttachment(KartSkeletalMeshComponent, FName("InstBoostLeft"));
	LeftInstantBoost->SetNetAddressable();
	LeftInstantBoost->SetIsReplicated(true);

	RightInstantBoost = CreateDefaultSubobject<UKartInstantBoostVFXComponent>(TEXT("InstBoostRight"));
	RightInstantBoost->SetupAttachment(KartSkeletalMeshComponent, FName("InstBoostRight"));
	RightInstantBoost->SetNetAddressable();
	RightInstantBoost->SetIsReplicated(true);

	LeftBoost = CreateDefaultSubobject<UKartPowerBoosterVFXComponent>(TEXT("BoostLeft"));
	LeftBoost->SetupAttachment(KartSkeletalMeshComponent, FName("BoostLeft"));
	LeftBoost->SetNetAddressable();
	LeftBoost->SetIsReplicated(true);
	
	RightBoost = CreateDefaultSubobject<UKartPowerBoosterVFXComponent>(TEXT("BoostRight"));
	RightBoost->SetupAttachment(KartSkeletalMeshComponent, FName("BoostRight"));
	RightBoost->SetNetAddressable();
	RightBoost->SetIsReplicated(true);
	
	AirBoost = CreateDefaultSubobject<UKartAirBoostVFXComponent>(TEXT("AirBoost"));
	AirBoost->SetupAttachment(KartSkeletalMeshComponent, FName("AirBoost"));
	AirBoost->SetNetAddressable();
	AirBoost->SetIsReplicated(true);

	Shield = CreateDefaultSubobject<UKartShieldVFXComponent>(TEXT("Shield"));
	Shield->SetupAttachment(KartSkeletalMeshComponent);
	Shield->SetNetAddressable();
	Shield->SetIsReplicated(true);

	static ConstructorHelpers::FClassFinder<USpeedLineUI> WBP_SPEEDLINEUI
	(TEXT("/Game/UIs/SpeedLine/WBP_SpeedLineAllUI.WBP_SpeedLineAllUI_C"));
	if (WBP_SPEEDLINEUI.Succeeded())
	{
		SpeedLineUIClass = WBP_SPEEDLINEUI.Class;
	}

	static ConstructorHelpers::FClassFinder<UBoosterGaugeUI> WBP_BOOSTERGAUGEUI
	(TEXT("/Game/UIs/HUD/BoosterGauge/WBP_BoosterGaugeUI.WBP_BoosterGaugeUI_C"));
	if (WBP_BOOSTERGAUGEUI.Succeeded())
	{
		BoosterGaugeUIClass = WBP_BOOSTERGAUGEUI.Class;
	}
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
		SpeedLineUI = CreateWidget<USpeedLineUI>(GetWorld(), SpeedLineUIClass);
		if (SpeedLineUI)
		{
			SpeedLineUI->AddToViewport();
			SpeedLineUI->Init();
		}
		BoosterGaugeUI = CreateWidget<UBoosterGaugeUI>(GetWorld(), BoosterGaugeUIClass);
		if (BoosterGaugeUI)
		{
			BoosterGaugeUI->AddToViewport();
			BoosterGaugeUI->Init();
		}
	}
	else
	{
		RootBox->SetSimulatePhysics(false);
	}
}

void AKart::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKart, bUsingBooster);
}

// Called every frame
void AKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FString localstr = FCommonUtil::GetClassEnumKeyAsString(GetLocalRole());
	// FString remotestr = FCommonUtil::GetClassEnumKeyAsString(GetRemoteRole());
	// FVector temp = GetActorLocation();
	// temp.Z += 100.f;

	bool flag = true;
	// 로컬의 위치만 업데이트 됨
	if(ItemInteractionComponent->GetbIsInteraction() == false && IsLocallyControlled())
	{
		
		UKartSystemLibrary::CalculateNormalizedSpeedWithBox(RootBox, MaxSpeed);
		
		flag &= LR_Wheel->ProcessSuspension(LineTraceLocations[0]);
		flag &= RR_Wheel->ProcessSuspension(LineTraceLocations[1]);
		flag &= LF_Wheel->ProcessSuspension(LineTraceLocations[2]);
		flag &= RF_Wheel->ProcessSuspension(LineTraceLocations[3]);
		
		SteeringComponent->ProcessSteering();
		bool bDrift = FrictionComponent->GetbDrift();
		if (flag)
		{
			SteeringComponent->ProcessTorque(bDrift);
			AccelerationComponent->ProcessAcceleration(bCanMove);
			FrictionComponent->ProcessFriction();

			LeftSkidMark->ProcessSkidMark(bDrift);
			RightSkidMark->ProcessSkidMark(bDrift);
		}
		else
		{
			LeftSkidMark->ProcessSkidMark(false);
			RightSkidMark->ProcessSkidMark(false);
		}
		BoosterComponent->ProcessBooster(bUsingBooster);

		EngineSoundComponent->PlayKartEngineSound();
		DriftSoundComponent->PlayDriftSound(bDrift && flag);
	}
	else
	{
		LeftSkidMark->ProcessSkidMark(false);
		RightSkidMark->ProcessSkidMark(false);
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

void AKart::UpdateSpeedUI()
{
	FVector LinearVelocity = RootBox->GetPhysicsLinearVelocity();
	FVector ForwardVector = RootBox->GetForwardVector();
	float KartSpeed = FVector::DotProduct(ForwardVector, LinearVelocity);
	// float KartSpeed = RootBox->GetPhysicsLinearVelocity().Length();

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

void AKart::ClearAcceleration()
{
	AccelerationComponent->ClearAcceleration();
}

void AKart::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		if (SpeedLineUI)
		{
			SpeedLineUI->ClearTimer();
		}
	}
	Super::EndPlay(EndPlayReason);
}
