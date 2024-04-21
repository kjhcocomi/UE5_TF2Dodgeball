// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DBGameState.h"
#include "TF2Dodgeball.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBKillLogWidget.h"
#include "System/DBGameInstance.h"

void ADBGameState::SetCurrentGameState(EDBGameState GS)
{
	CurrentDBGameState = GS;
}

void ADBGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBGameState, CurrentDBGameState);
	DOREPLIFETIME(ADBGameState, BlueWinCount);
	DOREPLIFETIME(ADBGameState, RedWinCount);
}