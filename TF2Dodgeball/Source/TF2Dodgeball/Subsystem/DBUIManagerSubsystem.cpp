// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBSelectTeamWidget.h"

UDBUIManagerSubsystem::UDBUIManagerSubsystem()
{
	static ConstructorHelpers::FClassFinder<UDBSelectTeamWidget> SelectTeamWidgetAsset(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_SelectTeam.WBP_SelectTeam_C'")
	);

	if (SelectTeamWidgetAsset.Succeeded())
	{
		SelectTeamWidgetClass = SelectTeamWidgetAsset.Class;
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
	if (IsValid(SelectTeamWidgetClass))
	{
		SelectTeamWidget = CreateWidget<UDBSelectTeamWidget>(GetWorld(), SelectTeamWidgetClass);
		if (IsValid(SelectTeamWidget))
		{
			SelectTeamWidget->AddToViewport();
			SelectTeamWidget->ShowUI();
		}
	}
}
