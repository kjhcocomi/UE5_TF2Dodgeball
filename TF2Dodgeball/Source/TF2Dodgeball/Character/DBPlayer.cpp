// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DBPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "System/DBGameInstance.h"
#include "GameMode/DBGameModeBase.h"

ADBPlayer::ADBPlayer()
{
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetRelativeRotation(FRotator(-50, 0, 0));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	SetThirdPersonView();
}

void ADBPlayer::BeginPlay()
{
	Super::BeginPlay();

	// tmp
	DBTeamColor = TeamColor::Blue;

	Cast<IDBGameInterface>(GetWorld()->GetAuthGameMode())->SpawnRocket();
}

void ADBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADBPlayer::SetThirdPersonView()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Camera
	Camera->bUsePawnControlRotation = false;

	// Spring Arm
	SpringArm->TargetArmLength = 500.f;

	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
}

void ADBPlayer::SetFirstPersonView()
{
}
