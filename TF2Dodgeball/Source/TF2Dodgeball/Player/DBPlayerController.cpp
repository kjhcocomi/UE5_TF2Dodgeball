// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/DBCharacter.h"
#include "Animation/DBAnimInstance.h"
#include "TF2Dodgeball.h"

ADBPlayerController::ADBPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ADBPlayerController::BeginPlay()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("End"));

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
		EnhancedInputComponent->BindAction(AirBlastAction, ETriggerEvent::Started, this, &ThisClass::AirBlast);
	}
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
	if (AirBlastMontage)
	{
		GetCharacter()->PlayAnimMontage(AirBlastMontage);
	}
	Cast<ADBCharacter>(GetCharacter())->AirBlast();
}
