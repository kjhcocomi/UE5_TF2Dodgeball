// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBScoreBoardWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DBPlayerController.h"
#include "Character/DBPlayer.h"
#include "GameMode/DBGameState.h"
#include "Utils/DBEnums.h"
#include "EngineUtils.h"
#include "UI/DBScoreBoardElementWidget.h"

UDBScoreBoardWidget::UDBScoreBoardWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UDBScoreBoardElementWidget> FindScoreBoardElementWidgetClass(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_ScoreBoardElement.WBP_ScoreBoardElement_C'")
	);

	if (FindScoreBoardElementWidgetClass.Succeeded())
	{
		ScoreBoardElementClass = FindScoreBoardElementWidgetClass.Class;
	}
}

void UDBScoreBoardWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBScoreBoardWidget::ShowUI()
{
	Super::ShowUI();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;

	for (int i = 0; i < ScoreBoardElementWidgets.Num(); i++)
	{
		ScoreBoardElementWidgets[i]->RemoveFromViewport();
	}
	ScoreBoardElementWidgets.Empty();

	// My Info
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (DBPC)
	{
		ADBCharacter* DBC = Cast<ADBCharacter>(DBPC->GetPawn());
		if (DBC)
		{
			FString Name = DBC->PlayerName;
			int KillCount = DBC->KillCount;
			int DeathCount = DBC->DeathCount;

			FString KillString = FString::Printf(TEXT("Kill : ")) + FString::FromInt(KillCount);
			FString DeathString = FString::Printf(TEXT("Death : ")) + FString::FromInt(DeathCount);

			Text_Name->SetText(FText::FromString(Name));
			Text_Kill->SetText(FText::FromString(KillString));
			Text_Death->SetText(FText::FromString(DeathString));

			if (DBC->GetTeamColor() == TeamColor::Blue)
			{
				Image_Pyro_Blue->SetVisibility(ESlateVisibility::Visible);
				Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (DBC->GetTeamColor() == TeamColor::Red)
			{
				Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
				Image_Pyro_Red->SetVisibility(ESlateVisibility::Visible);
			}
			else if (DBC->GetTeamColor() == TeamColor::Spectate)
			{
				Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
				Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

	// Team Score
	ADBGameState* DBGS = Cast<ADBGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (DBGS)
	{
		int ScoreBlue = DBGS->BlueWinCount;
		int ScoreRed = DBGS->RedWinCount;
		Text_ScoreBlue->SetText(FText::FromString(FString::FromInt(ScoreBlue)));
		Text_ScoreRed->SetText(FText::FromString(FString::FromInt(ScoreRed)));
	}

	// Other Players Info
	// TODO
	bool FirstSpectator = true;
	FString SpectatorString = FString::Printf(TEXT("Spectators : "));
	for (ADBCharacter* DBCharacter : TActorRange<ADBCharacter>(GetWorld()))
	{
		if (DBCharacter)
		{
			FString PlayerName = DBCharacter->PlayerName;
			int KillCount = DBCharacter->KillCount;
			int DeathCount = DBCharacter->DeathCount;
			TeamColor DBTeam = DBCharacter->PlayerTeamColor;
			bool IsLocalPlayer = DBCharacter->IsLocallyControlled();
			bool IsDead =  (DBCharacter->DBCharacterStateLocal == DBCharacterState::Spectate) || (DBCharacter->DBCharacterStateLocal == DBCharacterState::Dead);
			switch (DBTeam)
			{
			case TeamColor::Spectate:
			{
				if (FirstSpectator)
				{
					FirstSpectator = false;
				}
				else
				{
					SpectatorString += FString::Printf(TEXT(", "));
				}
				SpectatorString += PlayerName;
			}
				break;
			default:
			{
				UDBScoreBoardElementWidget* ElementWidget = CreateWidget<UDBScoreBoardElementWidget>(GetWorld(), ScoreBoardElementClass);
				ScoreBoardElementWidgets.Add(ElementWidget);
				ElementWidget->SetElement(PlayerName, KillCount, DeathCount, DBTeam, IsLocalPlayer, IsDead);
				if (DBTeam == TeamColor::Blue)VerticalBox_Blue->AddChildToVerticalBox(ElementWidget);
				if (DBTeam == TeamColor::Red)VerticalBox_Red->AddChildToVerticalBox(ElementWidget);
			}
				break;
			}
		}
	}
	Text_Spectators->SetText(FText::FromString(SpectatorString));
}

void UDBScoreBoardWidget::HideUI()
{
	Super::HideUI();
}
