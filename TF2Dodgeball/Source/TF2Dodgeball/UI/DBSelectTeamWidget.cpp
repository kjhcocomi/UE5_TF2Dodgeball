// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBSelectTeamWidget.h"
#include "Components/Button.h"
#include "Player/DBPlayerController.h"
#include "Character/DBPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "TF2Dodgeball.h"

UDBSelectTeamWidget::UDBSelectTeamWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBSelectTeamWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Spectator->OnClicked.AddDynamic(this, &UDBSelectTeamWidget::PushSpectatorButton);
	Button_Red->OnClicked.AddDynamic(this, &UDBSelectTeamWidget::PushRedButton);
	Button_Blue->OnClicked.AddDynamic(this, &UDBSelectTeamWidget::PushBlueButton);
}

void UDBSelectTeamWidget::PushSpectatorButton()
{
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (DBPC)
	{
		ADBPlayer* DBPlayer = Cast<ADBPlayer>(DBPC->GetPawn());
		if (DBPlayer)
		{
			DBPlayer->ServerRPCSetTeam(TeamColor::Spectate);
			HideUI();
		}
	}
}

void UDBSelectTeamWidget::PushRedButton()
{
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (DBPC)
	{
		ADBPlayer* DBPlayer = Cast<ADBPlayer>(DBPC->GetPawn());
		if (DBPlayer)
		{
			DBPlayer->ServerRPCSetTeam(TeamColor::Red);
			HideUI();
		}
	}
}

void UDBSelectTeamWidget::PushBlueButton()
{
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (DBPC)
	{
		ADBPlayer* DBPlayer = Cast<ADBPlayer>(DBPC->GetPawn());
		if (DBPlayer)
		{
			DBPlayer->ServerRPCSetTeam(TeamColor::Blue);
			HideUI();
		}
	}
}
