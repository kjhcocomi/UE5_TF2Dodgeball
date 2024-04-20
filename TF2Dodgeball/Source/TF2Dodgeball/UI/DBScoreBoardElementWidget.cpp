// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBScoreBoardElementWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UDBScoreBoardElementWidget::UDBScoreBoardElementWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBScoreBoardElementWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBScoreBoardElementWidget::SetElement(FString InName, int InKill, int InDeath, TeamColor InTeamColor, bool IsMyCharacter, bool IsDead)
{
	FText NameText = FText::FromString(InName);
	FText KillText = FText::FromString(FString::FromInt(InKill));
	FText DeathText = FText::FromString(FString::FromInt(InDeath));

	Text_Name->SetText(NameText);
	Text_Kill->SetText(KillText);
	Text_Death->SetText(DeathText);

	int ColorValue = 255;
	if (IsDead) ColorValue = 60;
	if (InTeamColor == TeamColor::Blue)
	{
		Text_Name->SetColorAndOpacity(FSlateColor(FColor(40, 40, ColorValue)));
		Text_Kill->SetColorAndOpacity(FSlateColor(FColor(40, 40, ColorValue)));
		Text_Death->SetColorAndOpacity(FSlateColor(FColor(40, 40, ColorValue)));
	}
	else if (InTeamColor == TeamColor::Red)
	{
		Text_Name->SetColorAndOpacity(FSlateColor(FColor(ColorValue, 40, 40)));
		Text_Kill->SetColorAndOpacity(FSlateColor(FColor(ColorValue, 40, 40)));
		Text_Death->SetColorAndOpacity(FSlateColor(FColor(ColorValue, 40, 40)));
	}

	if (IsMyCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("LocalCharacter"));
		Image_Background->SetBrushTintColor(FSlateColor(FColor(100, 100, 100, 100)));
	}
}


