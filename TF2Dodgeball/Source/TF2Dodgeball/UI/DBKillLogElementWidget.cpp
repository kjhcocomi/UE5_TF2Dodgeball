// Fill out your copyright notice in the Description page of Project Settings.


#include "DBKillLogElementWidget.h"
#include "Character/DBCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Utils/DBEnums.h"

UDBKillLogElementWidget::UDBKillLogElementWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBKillLogElementWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBKillLogElementWidget::SetElement(ADBCharacter* InAttacker, ADBCharacter* InVictim)
{
	FString AttackerName = FString::Printf(TEXT(""));
	FString VictimName = FString::Printf(TEXT(""));

	bool RelatedToMe = false;

	if (InAttacker)
	{
		AttackerName = InAttacker->PlayerName;
		RelatedToMe |= InAttacker->IsLocallyControlled();

		TeamColor ColorLocal = InAttacker->PlayerTeamColor;
		if (ColorLocal == TeamColor::Blue)
		{
			Text_Attacker->SetColorAndOpacity(FColor(40, 40, 255));
		}
		else if (ColorLocal == TeamColor::Red)
		{
			Text_Attacker->SetColorAndOpacity(FColor(255, 40, 40));
		}
	}
	if (InVictim)
	{
		VictimName = InVictim->PlayerName;
		RelatedToMe |= InVictim->IsLocallyControlled();

		TeamColor ColorLocal = InVictim->PlayerTeamColor;
		if (ColorLocal == TeamColor::Blue)
		{
			Text_Victim->SetColorAndOpacity(FColor(40, 40, 255));
		}
		else if (ColorLocal == TeamColor::Red)
		{
			Text_Victim->SetColorAndOpacity(FColor(255, 40, 40));
		}
	}

	if (RelatedToMe)
	{
		Image_Background->SetBrushTintColor(FSlateColor(FColor(180, 180, 180)));
	}

	Text_Attacker->SetText(FText::FromString(AttackerName));
	Text_Victim->SetText(FText::FromString(VictimName));

	GetWorld()->GetTimerManager().SetTimer(RemoveHandle, this, &UDBKillLogElementWidget::Remove, 5.f, false);

}

void UDBKillLogElementWidget::Remove()
{
	RemoveFromViewport();
}
