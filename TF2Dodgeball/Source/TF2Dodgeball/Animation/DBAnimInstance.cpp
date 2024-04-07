// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DBAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/DBCharacter.h"

UDBAnimInstance::UDBAnimInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ADBCharacter>(TryGetPawnOwner());

	if (Character)
		MovementComponent = Character->GetCharacterMovement();
}

void UDBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Character == nullptr)
		return;

	if (MovementComponent == nullptr)
		return;

	Velocity = MovementComponent->Velocity;
	GroundSpeed = Velocity.Size2D();

	bShouldMove = (GroundSpeed > 3.f && MovementComponent->GetCurrentAcceleration() != FVector::ZeroVector);
	bIsFalling = MovementComponent->IsFalling();
}
