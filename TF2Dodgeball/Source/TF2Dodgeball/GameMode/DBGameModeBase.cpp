// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DBGameModeBase.h"
#include "Pawn/DBRocket.h"
#include "Kismet/GameplayStatics.h"
#include "Character/DBCharacter.h"
#include "TF2Dodgeball.h"
#include "GameMode/DBGameState.h"
#include "EngineUtils.h"
#include "Player/DBPlayerController.h"

ADBGameModeBase::ADBGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GameStateClass = ADBGameState::StaticClass();
}

void ADBGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	DodgeBallGameState = Cast<ADBGameState>(UGameplayStatics::GetGameState(this));
	if (DodgeBallGameState)
	{
		DodgeBallGameState->SetCurrentGameState(EDBGameState::Wait);
	}
}

void ADBGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GatherCharacterInfo();

	switch (DodgeBallGameState->GetCurrentGameState())
	{
		case EDBGameState::Wait:
			Wait();
			break;
		case EDBGameState::Ready:
			Ready();
			break;
		case EDBGameState::Progress:
			Progress();
			break;
		case EDBGameState::Finish:
			Finish();
			break;
	}
}

void ADBGameModeBase::ChangeRocketOwnerTeam()
{
	if (RocketOwnerTeam == TeamColor::Blue)
	{
		RocketOwnerTeam = TeamColor::Red;
	}
	else
	{
		RocketOwnerTeam = TeamColor::Blue;
	}
}

TeamColor ADBGameModeBase::GetRocketOwnerTeam()
{
	return RocketOwnerTeam;
}

void ADBGameModeBase::SpawnRocket()
{
	const FTransform SpawnTransform(FVector::UpVector * 50.f);
	DBRocket = GetWorld()->SpawnActorDeferred<ADBRocket>(RocketClass, SpawnTransform);
	if (DBRocket == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Spawn Failed"));
	}
	else {
		bRocketValid = true;
		DBRocket->OnDestroyed.AddDynamic(this, &ADBGameModeBase::OnRocketDestroyed);
		DBRocket->FinishSpawning(SpawnTransform);
	}
}

void ADBGameModeBase::OnRocketDestroyed(AActor* DestroyedActor)
{
	// TODO : 사람 남아있는지 검사, 한팀만 남을경우 Ready로 변환
	bRocketValid = false;
	//SpawnRocket();
}

void ADBGameModeBase::GatherCharacterInfo()
{
	// Gather PlayerControllers
	DBPlayerControllers.Empty();
	for (ADBPlayerController* PlayerController : TActorRange<ADBPlayerController>(GetWorld()))
	{
		if (PlayerController)
		{
			DBPlayerControllers.Add(PlayerController);
		}
	}

	DBCharacters.Empty();
	DBBlueCharacters.Empty();
	DBRedCharacters.Empty();

	for (int i = 0; i < DBPlayerControllers.Num(); i++)
	{
		ADBCharacter* DBCharacter = Cast<ADBCharacter>(DBPlayerControllers[i]->GetPawn());
		if (DBCharacter)
		{
			DBCharacters.Add(DBCharacter);
		}
	}
	for (int i = 0; i < DBCharacters.Num(); i++)
	{
		if (DBCharacters[i]->GetTeamColor() == TeamColor::Blue) DBBlueCharacters.Add(DBCharacters[i]);
		if (DBCharacters[i]->GetTeamColor() == TeamColor::Red) DBRedCharacters.Add(DBCharacters[i]);
	}
}

void ADBGameModeBase::Wait()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Wait"));
	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Wait"));
	// 최소 레드1, 블루1이면 Ready상태로 변환
	if (bRocketValid)
	{
		DBRocket->Destroy();
	}
	if (DBBlueCharacters.Num() >= 1 && DBRedCharacters.Num() >= 1)
	{
		DodgeBallGameState->SetCurrentGameState(EDBGameState::Ready);
	}
	else // 인원부족
	{
		for (int i = 0; i < DBCharacters.Num(); i++)
		{
			ADBPlayerController* DBPC = Cast<ADBPlayerController>(DBCharacters[i]->GetController());
			if (DBPC) // 플레이어라면 (봇이 아니라면)
			{
				if (DBCharacters[i]->PlayerTeamColor != TeamColor::Spectate)
				{
					DBCharacters[i]->Spectate();
				}
			}
		}
	}
}

void ADBGameModeBase::Ready()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Ready"));
	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Ready"));
	if (DBBlueCharacters.Num() == 0 || DBRedCharacters.Num() == 0)
	{
		// 대기중에 레드0이거나 블루0되면 다시 Wait상태로 변환
		// Ready -> Wait
		DodgeBallGameState->SetCurrentGameState(EDBGameState::Wait);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ready);
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Ready) == false)
		{
			// 몇초 후 Progress 상태로 변하도록 예약
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ready, this, &ADBGameModeBase::ReadyToProgress, 5.f, false);
		}
		for (int i = 0; i < DBCharacters.Num(); i++)
		{
			// 죽어있는 캐릭터들 부활
			if (DBCharacters[i]->PlayerTeamColor != TeamColor::Spectate)
			{
				DBCharacters[i]->Revive();
			}
		}
	}
}

void ADBGameModeBase::Progress()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Progress"));
	//DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Progress"));
	int AliveBlueCnt = 0;
	int AliveRedCnt = 0;
	for (int i = 0; i < DBBlueCharacters.Num(); i++)
	{
		if (DBBlueCharacters[i]->GetCharacterState() == DBCharacterState::Alive) AliveBlueCnt++;
	}
	for (int i = 0; i < DBRedCharacters.Num(); i++)
	{
		if (DBRedCharacters[i]->GetCharacterState() == DBCharacterState::Alive) AliveRedCnt++;
	}
	// TODO : 레드0이거나 블루0되면 Wait상태로 변환
	if (AliveBlueCnt >= 1 && AliveRedCnt >= 1)
	{
		if (bRocketValid == false)
		{
			SpawnRocket();
		}
	}
	else
	{
		if (bRocketValid)
		{
			DBRocket->Destroy();
		}
		DodgeBallGameState->SetCurrentGameState(EDBGameState::Finish);
	}
}

void ADBGameModeBase::Finish()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Finish"));
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Finish) == false)
	{
		// 몇초 후 Ready 상태로
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Finish, this, &ADBGameModeBase::FinishToReady, 5.f, false);
	}
}

void ADBGameModeBase::ReadyToProgress()
{
	DodgeBallGameState->SetCurrentGameState(EDBGameState::Progress);
	for (int i = 0; i < DBCharacters.Num(); i++)
	{
		if (DBCharacters[i]->PlayerTeamColor != TeamColor::Spectate)
		{
			DBCharacters[i]->StartGame();
		}
	}
}

void ADBGameModeBase::FinishToReady()
{
	DodgeBallGameState->SetCurrentGameState(EDBGameState::Ready);
}

const TArray<ADBCharacter*>& ADBGameModeBase::GetCharacters()
{
	return DBCharacters;
}

const TArray<class ADBCharacter*>& ADBGameModeBase::GetBlueCharacters()
{
	return DBBlueCharacters;
}

const TArray<class ADBCharacter*>& ADBGameModeBase::GetRedCharacters()
{
	return DBRedCharacters;
}