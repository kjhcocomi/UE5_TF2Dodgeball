// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DBGameState.h"
#include "TF2Dodgeball.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBKillLogWidget.h"
#include "System/DBGameInstance.h"
#include "UI/DBChatWidget.h"
#include "UI/DBChatTmpWidget.h"

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

void ADBGameState::MulticastRPCBroadCastMessage_Implementation(ADBCharacter* Sender, const FText& Text)
{
	if (HasAuthority() == false)
	{
		UDBUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UDBUIManagerSubsystem>();
		if (UIManager)
		{
			UDBChatWidget* ChatUI = UIManager->GetChatWidget();
			if (ChatUI)
			{
				ChatUI->AddChat(Sender, Text);
			}
			UDBChatTmpWidget* ChatTmpUI = UIManager->GetChatTmpWidget();
			if (ChatTmpUI)
			{
				ChatTmpUI->AddChat(Sender, Text);
			}
		}
	}
}