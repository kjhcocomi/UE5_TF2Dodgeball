// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/DBRocket.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Character/DBPlayer.h"
#include "Character/DBCharacter.h"
#include "System/DBGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/DBGameInterface.h"
#include "GameMode/DBGameModeBase.h"

// Sets default values
ADBRocket::ADBRocket()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloaingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloaingPawnMovement"));
}

// Called when the game starts or when spawned
void ADBRocket::BeginPlay()
{
	Super::BeginPlay();
	FindTargetPlayer();
	GetMovementComponent()->Activate(true);
}

// Called every frame
void ADBRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetCharacter != nullptr) 
	{
		FVector TargetPosition = TargetCharacter->GetController()->GetPawn()->GetActorLocation();
		FVector MyPosition = GetActorLocation();

		FVector TargetDirection = TargetPosition - MyPosition;
		TargetDirection.Normalize();

		CurrentDirection = CurrentDirection + TargetDirection * DeltaTime * 5;
		CurrentDirection.Normalize();

		//AddActorWorldOffset(CurrentDirection * FloaingPawnMovement->MaxSpeed / 100.f);

		AddMovementInput(CurrentDirection, 1);
		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, CurrentDirection.ToString());
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("No Target"));
		FindTargetPlayer();
	}
}

// Called to bind functionality to input
void ADBRocket::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADBRocket::FindTargetPlayer()
{
	IDBGameInterface* DBGameMode = Cast<IDBGameInterface>(GetWorld()->GetAuthGameMode());
	TeamColor TargetTeam = DBGameMode->GetCurrentTargetTeam();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADBCharacter::StaticClass(), Actors);

	TArray<ADBCharacter*> DBCharacters;
	for (int i = 0; i < Actors.Num(); i++) {
		ADBCharacter* DBCharacter = Cast<ADBCharacter>(Actors[i]);
		if (DBCharacter == nullptr) continue;
		if (DBCharacter->GetTeamColor() != TargetTeam) continue;
		DBCharacters.Add(DBCharacter);
	}

	// tmp
	if (DBCharacters.IsEmpty() == false)
	{
		TargetCharacter = DBCharacters[0];
	}
}

void ADBRocket::Explode()
{
	// TODO : ·ÎÄÏ Æø¹ß
	Destroy();
}

void ADBRocket::Reflect()
{
	IDBGameInterface* DBGameInterface = Cast<IDBGameInterface>(GetWorld()->GetAuthGameMode());
	DBGameInterface->ChangeTargetTeam();

	FloaingPawnMovement->MaxSpeed = FloaingPawnMovement->MaxSpeed + 300.f;
	FindTargetPlayer();
}

void ADBRocket::SetCurrentDirection(FVector Direction)
{
	Direction.Normalize();
	CurrentDirection = Direction;
}
