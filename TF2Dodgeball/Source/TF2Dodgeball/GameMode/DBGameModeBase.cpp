// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DBGameModeBase.h"
#include "Pawn/DBRocket.h"
#include "Kismet/GameplayStatics.h"
#include "Character/DBCharacter.h"

ADBGameModeBase::ADBGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADBGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GatherCharacterInfo();

	switch (CurrGameState)
	{
		case DBGameState::Wait:
			Wait();
			break;
		case DBGameState::Ready:
			Ready();
			break;
		case DBGameState::Progress:
			Progress();
			break;
		case DBGameState::Finish:
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
	// TODO : ��� �����ִ��� �˻�, ������ ������� Ready�� ��ȯ
	bRocketValid = false;
	//SpawnRocket();
}

void ADBGameModeBase::GatherCharacterInfo()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADBCharacter::StaticClass(), Actors);

	DBCharacters.Empty();
	DBBlueCharacters.Empty();
	DBRedCharacters.Empty();

	for (int i = 0; i < Actors.Num(); i++) {
		ADBCharacter* DBCharacter = Cast<ADBCharacter>(Actors[i]);
		if (DBCharacter == nullptr) continue;
		DBCharacters.Add(DBCharacter);
	}
	for (int i = 0; i < DBCharacters.Num(); i++)
	{
		if (DBCharacters[i]->GetTeamColor() == TeamColor::Blue) DBBlueCharacters.Add(DBCharacters[i]);
		if (DBCharacters[i]->GetTeamColor() == TeamColor::Red) DBRedCharacters.Add(DBCharacters[i]);
	}
}

void ADBGameModeBase::Wait()
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Wait"));
	// TODO : �ּ� ����1, ���1�̸� Ready���·� ��ȯ
	if (DBBlueCharacters.Num() >= 1 && DBRedCharacters.Num() >= 1)
	{
		CurrGameState = DBGameState::Ready;
	}
	else // �ο�����
	{
		for (int i = 0; i < DBCharacters.Num(); i++)
		{
			DBCharacters[i]->Spectate();
		}
	}
}

void ADBGameModeBase::Ready()
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Ready"));
	// TODO : �׾��ִ� ĳ���͵� ��Ȱ
	// TODO : ���� �� Progress ���·� ���ϵ��� ����
	// TODO : ����߿� ����0�̰ų� ���0�Ǹ� �ٽ� Wait���·� ��ȯ
	if (DBBlueCharacters.Num() == 0 || DBRedCharacters.Num() == 0)
	{
		// Ready -> Wait
		
		CurrGameState = DBGameState::Wait;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ready);
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Ready) == false)
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ready, this, &ADBGameModeBase::ReadyToProgress, 5.f, false);
		}
		for (int i = 0; i < DBCharacters.Num(); i++)
		{
			DBCharacters[i]->Revive();
		}
	}
}

void ADBGameModeBase::Progress()
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Progress"));
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
	// TODO : ����0�̰ų� ���0�Ǹ� Wait���·� ��ȯ
	if (AliveBlueCnt >= 1 && AliveRedCnt >= 1)
	{
		if (bRocketValid == false)
		{
			SpawnRocket();
		}
	}
	else
	{
		CurrGameState = DBGameState::Finish;
	}
}

void ADBGameModeBase::Finish()
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Finish"));
	if (GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Finish) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Finish, this, &ADBGameModeBase::FinishToReady, 5.f, false);
	}
}

void ADBGameModeBase::ReadyToProgress()
{
	CurrGameState = DBGameState::Progress;
	for (int i = 0; i < DBCharacters.Num(); i++)
	{
		DBCharacters[i]->StartGame();
	}
}

void ADBGameModeBase::FinishToReady()
{
	CurrGameState = DBGameState::Ready;
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