// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DBGameModeBase.h"
#include "Pawn/DBRocket.h"

ADBGameModeBase::ADBGameModeBase()
{
}

void ADBGameModeBase::ChangeTargetTeam()
{
	if (CurrentTargetTeam == TeamColor::Blue)
	{
		CurrentTargetTeam = TeamColor::Red;
	}
	else
	{
		CurrentTargetTeam = TeamColor::Blue;
	}
}

TeamColor ADBGameModeBase::GetCurrentTargetTeam()
{
	return CurrentTargetTeam;
}

void ADBGameModeBase::SpawnRocket()
{
	const FTransform SpawnTransform(FVector::UpVector * 50.f);
	ADBRocket* RocketInstance = GetWorld()->SpawnActorDeferred<ADBRocket>(RocketClass, SpawnTransform);
	if (RocketInstance == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("Spawn Failed"));
	}
	else {
		RocketInstance->OnDestroyed.AddDynamic(this, &ADBGameModeBase::OnRocketDestroyed);
		RocketInstance->FinishSpawning(SpawnTransform);
	}
}

void ADBGameModeBase::OnRocketDestroyed(AActor* DestroyedActor)
{
	// TODO : 사람 남아있는지 검사
	SpawnRocket();
}
