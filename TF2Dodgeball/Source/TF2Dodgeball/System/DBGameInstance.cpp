// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DBGameInstance.h"
#include "Pawn/DBRocket.h"

UDBGameInstance::UDBGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> DBRocket(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_DBRocket.BP_DBRocket'"));
	if (DBRocket.Object)
	{
		RocketBlueprint = (UClass*)DBRocket.Object->GeneratedClass;
	}
}

void UDBGameInstance::Init()
{
	Super::Init();
	SpawnRocket();
	
}

void UDBGameInstance::Shutdown()
{
	Super::Shutdown();
}

const TeamColor& UDBGameInstance::GetCurrentTargetTeam()
{
	return CurrentTargetTeam;
}

void UDBGameInstance::ChangeTargetTeam()
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

void UDBGameInstance::SpawnRocket()
{
	GetWorld()->SpawnActor<ADBRocket>(RocketBlueprint, RocketSpawnLocation, FRotator::ZeroRotator);
}

void UDBGameInstance::Enter()
{
}

void UDBGameInstance::Exit()
{
}
