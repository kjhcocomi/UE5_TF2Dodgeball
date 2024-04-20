// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/DBCharacter.h"
#include "Animation/DBAnimInstance.h"
#include "TF2Dodgeball.h"
#include "Subsystem/DBUIManagerSubsystem.h"

ADBPlayerController::ADBPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ADBPlayerController::PostInitializeComponents()
{
	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostInitializeComponents();

	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));

}

void ADBPlayerController::PostNetInit()
{
	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::PostNetInit();

	/*UNetDriver* NetDriver = GetNetDriver();
	if (NetDriver)
	{
		if(NetDriver->ServerConnection)
			DB_LOG(LogDBNetwork, Log, TEXT("Server Connection : %s"), *NetDriver->ServerConnection->GetName());
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("No NetDriver"));
	}

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));*/
}

void ADBPlayerController::BeginPlay()
{
	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));

	auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ADBPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ThisClass::Turn);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJump);
		EnhancedInputComponent->BindAction(AirBlastAction, ETriggerEvent::Triggered, this, &ThisClass::AirBlast);
		EnhancedInputComponent->BindAction(ShowScoreBoardAction, ETriggerEvent::Started, this, &ThisClass::ShowScore);
		EnhancedInputComponent->BindAction(ShowScoreBoardAction, ETriggerEvent::Completed, this, &ThisClass::HideScore);
		EnhancedInputComponent->BindAction(ShowSelectTeamUIAction, ETriggerEvent::Started, this, &ThisClass::ShowSelectTeamUI);
		EnhancedInputComponent->BindAction(ShowMenuUIAction, ETriggerEvent::Started, this, &ThisClass::ShowMenuUI);
	}
}

void ADBPlayerController::OnPossess(APawn* InPawn)
{
	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnPossess(InPawn);

	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));
}

void ADBPlayerController::Move(const FInputActionValue& InputValue)
{
	FVector2D MoveInput = InputValue.Get<FVector2D>();

	UDBAnimInstance* DBAnimInstance = Cast<UDBAnimInstance>(GetCharacter()->GetMesh()->GetAnimInstance());
	if (DBAnimInstance)
	{
		DBAnimInstance->MoveInput = MoveInput.GetSafeNormal();
	}

	if (MoveInput.X != 0)
	{
		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetForwardVector(FRotator(0, Rotator.Yaw, 0));
		GetPawn()->AddMovementInput(Direction, MoveInput.X);
	}
	if (MoveInput.Y != 0)
	{
		FRotator Rotator = GetControlRotation();
		FVector Direction = UKismetMathLibrary::GetRightVector(FRotator(0, Rotator.Yaw, 0));
		GetPawn()->AddMovementInput(Direction, MoveInput.Y);
	}
}

void ADBPlayerController::Turn(const FInputActionValue& InputValue)
{
	FVector2D TurnInput = InputValue.Get<FVector2D>();
	AddYawInput(TurnInput.X);
	AddPitchInput(-TurnInput.Y);
}

void ADBPlayerController::Jump(const FInputActionValue& InputValue)
{
	GetCharacter()->Jump();
}

void ADBPlayerController::StopJump(const FInputActionValue& InputValue)
{
	GetCharacter()->StopJumping();
}

void ADBPlayerController::AirBlast(const FInputActionValue& InputValue)
{
	if (bCanAirBlast == false) return;
	bCanAirBlast = false;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AirBlast, this, &ADBPlayerController::CoolDown_AirBlast, 1.f, false);

	if (AirBlastMontage)
	{
		GetCharacter()->PlayAnimMontage(AirBlastMontage);
	}
	Cast<ADBCharacter>(GetCharacter())->AirBlast();
}

void ADBPlayerController::ShowScore(const FInputActionValue& InputValue)
{
	UDBUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UDBUIManagerSubsystem>();
	if (UIManager)
	{
		UIManager->ShowScoreBoardUI();
	}
}

void ADBPlayerController::HideScore(const FInputActionValue& InputValue)
{
	UDBUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UDBUIManagerSubsystem>();
	if (UIManager)
	{
		UIManager->HideScoreBoardUI();
	}
}

void ADBPlayerController::ShowSelectTeamUI(const FInputActionValue& InputValue)
{
	UDBUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UDBUIManagerSubsystem>();
	if (UIManager)
	{
		UIManager->ShowSelectTeamUI();
	}
}

void ADBPlayerController::ShowMenuUI(const FInputActionValue& InputValue)
{
	UE_LOG(LogTemp, Log, TEXT("ESC"));
}

void ADBPlayerController::CoolDown_AirBlast()
{
	bCanAirBlast = true;
}
