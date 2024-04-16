// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBUserWidget.h"

void UDBUserWidget::ShowUI()
{
	SetVisibility(ESlateVisibility::Visible);
}

void UDBUserWidget::HideUI()
{
	SetVisibility(ESlateVisibility::Hidden);
}
