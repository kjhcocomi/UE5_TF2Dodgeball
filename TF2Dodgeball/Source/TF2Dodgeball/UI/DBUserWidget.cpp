// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UDBUserWidget::ShowUI()
{
	SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;
}

void UDBUserWidget::HideUI()
{
	SetVisibility(ESlateVisibility::Hidden);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
}
