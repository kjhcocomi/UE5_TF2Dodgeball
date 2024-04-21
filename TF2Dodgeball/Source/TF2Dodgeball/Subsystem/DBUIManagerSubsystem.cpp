// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBSelectTeamWidget.h"
#include "UI/DBScoreBoardWidget.h"
#include "UI/DBKillLogWidget.h"

UDBUIManagerSubsystem::UDBUIManagerSubsystem()
{
	static ConstructorHelpers::FClassFinder<UDBSelectTeamWidget> SelectTeamWidgetAsset(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_SelectTeam.WBP_SelectTeam_C'")
	);

	static ConstructorHelpers::FClassFinder<UDBScoreBoardWidget> ScoreBoardWidgetAsset(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_ScoreBoard.WBP_ScoreBoard_C'")
	);

	static ConstructorHelpers::FClassFinder<UDBKillLogWidget> KillLogWidgetAsset(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_KillLog.WBP_KillLog_C'")
	);

	if (SelectTeamWidgetAsset.Succeeded())
	{
		SelectTeamWidgetClass = SelectTeamWidgetAsset.Class;
	}

	if (ScoreBoardWidgetAsset.Succeeded())
	{
		ScoreBoardWidgetClass = ScoreBoardWidgetAsset.Class;
	}

	if (KillLogWidgetAsset.Succeeded())
	{
		KillLogWidgetClass = KillLogWidgetAsset.Class;
	}
}

void UDBUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UDBUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDBUIManagerSubsystem::ShowSelectTeamUI()
{
	if (SelectTeamWidget == nullptr)
	{
		if (IsValid(SelectTeamWidgetClass))
		{
			SelectTeamWidget = CreateWidget<UDBSelectTeamWidget>(GetWorld(), SelectTeamWidgetClass);
			if (IsValid(SelectTeamWidget))
			{
				SelectTeamWidget->AddToViewport();
			}
		}
	}
	SelectTeamWidget->ShowUI();
}

void UDBUIManagerSubsystem::ShowScoreBoardUI()
{
	if (ScoreBoardWidget == nullptr)
	{
		if (IsValid(ScoreBoardWidgetClass))
		{
			ScoreBoardWidget = CreateWidget<UDBScoreBoardWidget>(GetWorld(), ScoreBoardWidgetClass);
			if (IsValid(ScoreBoardWidget))
			{
				ScoreBoardWidget->AddToViewport();
			}
		}
	}
	ScoreBoardWidget->ShowUI();
}

void UDBUIManagerSubsystem::HideScoreBoardUI()
{
	if (ScoreBoardWidget)
	{
		ScoreBoardWidget->HideUI();
	}
}

void UDBUIManagerSubsystem::ShowKillLogUI()
{
	if (KillLogWidget == nullptr)
	{
		if (IsValid(KillLogWidgetClass))
		{
			KillLogWidget = CreateWidget<UDBKillLogWidget>(GetWorld(), KillLogWidgetClass);
			if (IsValid(KillLogWidget))
			{
				KillLogWidget->AddToViewport();
			}
		}
	}
	KillLogWidget->ShowUI();
}
