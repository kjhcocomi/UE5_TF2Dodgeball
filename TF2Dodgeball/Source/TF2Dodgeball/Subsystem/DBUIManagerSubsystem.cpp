// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBSelectTeamWidget.h"
#include "UI/DBScoreBoardWidget.h"

UDBUIManagerSubsystem::UDBUIManagerSubsystem()
{
	static ConstructorHelpers::FClassFinder<UDBSelectTeamWidget> SelectTeamWidgetAsset(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_SelectTeam.WBP_SelectTeam_C'")
	);

	static ConstructorHelpers::FClassFinder<UDBScoreBoardWidget> ScoreBoardWidgetAsset(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_ScoreBoard.WBP_ScoreBoard_C'")
	);

	if (SelectTeamWidgetAsset.Succeeded())
	{
		SelectTeamWidgetClass = SelectTeamWidgetAsset.Class;
	}

	if (ScoreBoardWidgetAsset.Succeeded())
	{
		ScoreBoardWidgetClass = ScoreBoardWidgetAsset.Class;
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
