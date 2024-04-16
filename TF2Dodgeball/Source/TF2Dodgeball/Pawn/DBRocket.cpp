// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/DBRocket.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Character/DBPlayer.h"
#include "Character/DBCharacter.h"
#include "System/DBGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/DBGameInterface.h"
#include "GameMode/DBGameModeBase.h"
#include "Player/DBPlayerState.h"

// Sets default values
ADBRocket::ADBRocket()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FloaingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloaingPawnMovement"));
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADBRocket::BeginPlay()
{
	Super::BeginPlay();
	FindTargetPlayer();
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
		//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("No Target"));
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
	return;
	ADBGameModeBase* DBGameMode = Cast<ADBGameModeBase>(GetWorld()->GetAuthGameMode());
	TeamColor TargetTeam;
	TArray<ADBCharacter*> DBTargetCharacters;
	// 타겟 팀 변경, 타겟 캐릭터들 얻어옴
	if (AttackerTeam == TeamColor::None) 
	{
		// 서브공은 Owner에게 
		TargetTeam = DBGameMode->GetRocketOwnerTeam();
		if (TargetTeam == TeamColor::Blue) DBTargetCharacters = DBGameMode->GetBlueCharacters();
		else DBTargetCharacters = DBGameMode->GetRedCharacters();
	}
	else if (AttackerTeam == TeamColor::Blue)
	{
		TargetTeam = TeamColor::Red;
		DBTargetCharacters = DBGameMode->GetRedCharacters();
	}
	else
	{
		TargetTeam = TeamColor::Blue;
		DBTargetCharacters = DBGameMode->GetBlueCharacters();
	}
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, FString::Printf(TEXT("%d"), DBTargetCharacters.Num()));

	// 타겟 캐릭터들 중 살아있는 캐릭터만 추출
	TArray<ADBCharacter*> AliveTargetCharacters;
	for (int i = 0; i < DBTargetCharacters.Num(); i++)
	{
		if (DBTargetCharacters[i]->GetCharacterState() == DBCharacterState::Alive)
		{
			AliveTargetCharacters.Add(DBTargetCharacters[i]);
		}
	}

	// tmp
	if (AliveTargetCharacters.IsEmpty() == false)
	{
		TargetCharacter = AliveTargetCharacters[0];
	}
	else
	{
		TargetCharacter = nullptr;
	}
}

void ADBRocket::Explode(ADBCharacter* HittedCharacter)
{
	ADBGameModeBase* DBGameMode = Cast<ADBGameModeBase>(GetWorld()->GetAuthGameMode());
	if (DBGameMode)
	{
		// tmp
		if (AttackerTeam != HittedCharacter->GetTeamColor())
		{
			HittedCharacter->OnDamaged(this);
		}
		if (DBGameMode->GetRocketOwnerTeam() != AttackerTeam)
		{
			DBGameMode->ChangeRocketOwnerTeam();
		}
		// TODO : 폭발
		Destroy();
	}
}

void ADBRocket::Reflect(ADBCharacter* InAttacker)
{
	Attacker = InAttacker;
	AttackerTeam = Attacker->GetTeamColor();

	FloaingPawnMovement->MaxSpeed = FloaingPawnMovement->MaxSpeed + 300.f;
	FindTargetPlayer();
}

void ADBRocket::SetCurrentDirection(FVector Direction)
{
	Direction.Normalize();
	CurrentDirection = Direction;
}
