// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBHudWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Character/DBCharacter.h"
#include "Utils/DBEnums.h"

UDBHudWidget::UDBHudWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBHudWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBHudWidget::ShowUI()
{
	Super::ShowUI();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
	Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
	Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
	Image_Dead->SetVisibility(ESlateVisibility::Hidden);
}

void UDBHudWidget::SetPercent()
{
	CurrentSecond += TickSecond;
	float PercentValue = CurrentSecond / CoolTime;
	if (PercentValue >= 1.f)
	{
		PercentValue = 1.f;
		GetWorld()->GetTimerManager().ClearTimer(CoolDownHandle);
	}
	ProgressBar_CoolDown->SetPercent(PercentValue);
}

void UDBHudWidget::SetImage(ADBCharacter* Player)
{
	if (Player == nullptr)
	{
		Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
		Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
		Image_Dead->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (Player->DBCharacterStateLocal == DBCharacterState::None)
		{
			Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
			Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
			Image_Dead->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (Player->DBCharacterStateLocal == DBCharacterState::Dead || Player->DBCharacterStateLocal == DBCharacterState::Spectate)
		{
			Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
			Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
			Image_Dead->SetVisibility(ESlateVisibility::Visible);
			if (Player->GetTeamColor() == TeamColor::Spectate)
			{
				Image_Dead->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			if (Player->GetTeamColor() == TeamColor::None || Player->GetTeamColor() == TeamColor::Spectate)
			{
				Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
				Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
				Image_Dead->SetVisibility(ESlateVisibility::Hidden);
			}
			else if (Player->GetTeamColor() == TeamColor::Blue)
			{
				Image_Pyro_Blue->SetVisibility(ESlateVisibility::Visible);
				Image_Pyro_Red->SetVisibility(ESlateVisibility::Hidden);
				Image_Dead->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				Image_Pyro_Blue->SetVisibility(ESlateVisibility::Hidden);
				Image_Pyro_Red->SetVisibility(ESlateVisibility::Visible);
				Image_Dead->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UDBHudWidget::SetRocketSpeedText(float RocketSpeed)
{
	FString RocketSpeedStr = FString::Printf(TEXT(""));
	RocketSpeed /= 20;
	if (RocketSpeed > 0)
	{
		RocketSpeedStr = FString::Printf(TEXT("Speed : ")) + FString::FromInt((int)RocketSpeed);
	}
	Text_RocketSpeed->SetText(FText::FromString(RocketSpeedStr));
}

void UDBHudWidget::StartCoolDown(float InCoolTime)
{
	CurrentSecond = 0.f;
	CoolTime = InCoolTime;
	GetWorld()->GetTimerManager().SetTimer(CoolDownHandle, this, &UDBHudWidget::SetPercent, TickSecond, true, TickSecond);
}
