// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "TF2Dodgeball.h"
#include "Player/DBPlayerController.h"

void ADBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADBPlayerState, PlayerTeamColor);
	DOREPLIFETIME(ADBPlayerState, DBCharacterState);
}

void ADBPlayerState::OnRep_PlayerName()
{
	Super::OnRep_PlayerName();
	// TODO : UI 반영
}

void ADBPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	// TODO : UI 반영
}

void ADBPlayerState::OnRep_PlayerTeamColor()
{
}

void ADBPlayerState::OnRep_DBCharacterState()
{
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(GetOwner());
	if (DBPC)
	{
		if (DBPC->IsLocalController())
		{
			DB_LOG(LogDBNetwork, Log, TEXT("%s, %s"), TEXT("Local"), *GetName());
		}
		else
		{
			DB_LOG(LogDBNetwork, Log, TEXT("%s, %s"), TEXT("Other Player"), *GetName());
		}
	}
	else
	{
		DB_LOG(LogDBNetwork, Log, TEXT("%s, %s"), TEXT("No PC"), *GetName());
	}
	
	switch (DBCharacterState)
	{
		case DBCharacterState::None:
			break;
		case DBCharacterState::Dead:
			break;
		case DBCharacterState::Ready:
			break;
		case DBCharacterState::Alive:
			break;
		case DBCharacterState::Spectate:
			break;
	}
}
