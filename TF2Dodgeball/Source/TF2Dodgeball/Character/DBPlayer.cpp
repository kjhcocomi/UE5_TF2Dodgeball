// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DBPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "System/DBGameInstance.h"
#include "GameMode/DBGameModeBase.h"
#include "TF2Dodgeball.h"
#include "Player/DBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/DBSubsystem.h"
#include "Subsystem/DBUIManagerSubsystem.h"

ADBPlayer::ADBPlayer()
{
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	// tp
	Camera_Tpv = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_Tpv"));
	Camera_Tpv->SetupAttachment(SpringArm);

	Camera_Fpv = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera_Fpv"));
	Camera_Fpv->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetThirdPersonCamera();
	SetFirstPersonCamera();
}

void ADBPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		PlayerTeamColor = TeamColor::Spectate;
		DBCharacterStateLocal = DBCharacterState::Spectate;
	}
	if (!HasAuthority() && IsLocallyControlled())
	{
		UDBSubsystem* Ss = GetGameInstance()->GetSubsystem<UDBSubsystem>();
		if (Ss)
		{
			FString PlayerNameTemp = Ss->GetPlayerName();
			ServerRPCSetName(PlayerNameTemp);
		}

		UDBUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UDBUIManagerSubsystem>();
		if (UIManager)
		{
			UIManager->ShowHudUI();
			UIManager->ShowKillLogUI();
			UIManager->ShowChatTmpUI();
			UIManager->ShowSelectTeamUI();
		}
	}
}

void ADBPlayer::PossessedBy(AController* NewController)
{
	/*DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}*/

	Super::PossessedBy(NewController);

	/*OwnerActor = GetOwner();
	if (OwnerActor)
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));*/
}

void ADBPlayer::OnRep_Owner()
{
	//DB_LOG(LogDBNetwork, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	/*AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		DB_LOG(LogDBNetwork, Log, TEXT("Owner : %s"), *OwnerActor->GetName());
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No Owner"));
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));*/
}

void ADBPlayer::PostNetInit()
{
	//DB_LOG(LogDBNetwork, Log, TEXT("%s %s"), TEXT("Begin"), *GetName());
	
	Super::PostNetInit();

	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ADBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADBPlayer::ChangeView()
{
	if (bIsThirdPersonView)
	{
		Camera_Fpv->Activate();
		Camera_Tpv->Deactivate();
		bIsThirdPersonView = false;
	}
	else 
	{
		Camera_Fpv->Deactivate();
		Camera_Tpv->Activate();
		bIsThirdPersonView = true;
	}
}

void ADBPlayer::SetThirdPersonCamera()
{
	// Camera
	Camera_Tpv->bUsePawnControlRotation = false;
	Camera_Tpv->SetRelativeLocation(FVector(0, 0, 0));

	// Spring Arm
	SpringArm->SetRelativeRotation(FRotator(-50, 0, 0));
	SpringArm->TargetArmLength = 400.f;

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;

	bIsThirdPersonView = true;
}

void ADBPlayer::SetFirstPersonCamera()
{
	Camera_Fpv->bUsePawnControlRotation = true;
	Camera_Fpv->SetRelativeRotation(FRotator(-90, 0, 90));
	Camera_Fpv->bAutoActivate = false;
}
