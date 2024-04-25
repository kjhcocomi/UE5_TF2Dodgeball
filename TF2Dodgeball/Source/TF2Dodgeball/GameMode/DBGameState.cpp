// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DBGameState.h"
#include "TF2Dodgeball.h"
#include "Net/UnrealNetwork.h"
#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBKillLogWidget.h"
#include "System/DBGameInstance.h"
#include "UI/DBChatWidget.h"
#include "UI/DBChatTmpWidget.h"
#include "Character/DBCharacter.h"
#include "Player/DBPlayerController.h"

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

void ADBGameState::MulticastRPCReady_Implementation(int second)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle5, this, &ADBGameState::Count5, second - 5.f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle4, this, &ADBGameState::Count4, second - 4.f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle3, this, &ADBGameState::Count3, second - 3.f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle2, this, &ADBGameState::Count2, second - 2.f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle1, this, &ADBGameState::Count1, second - 1.f, false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle0, this, &ADBGameState::Count0, second - 0.f, false);
}

void ADBGameState::MulticastRPCReadyCancel_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle5);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle4);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle3);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle2);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle1);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle0);
}

void ADBGameState::MulticastRPCGameResult_Implementation(TeamColor WinnerTeam)
{
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (DBPC)
	{
		ADBCharacter* DBC = Cast<ADBCharacter>(DBPC->GetPawn());
		if (DBC)
		{
			TeamColor PTC = DBC->PlayerTeamColor;
			if (WinnerTeam == PTC)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), VictorySound, FVector(0, 0, 0));
			}
			else
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), LoseSound, FVector(0, 0, 0));
			}
		}
	}
}