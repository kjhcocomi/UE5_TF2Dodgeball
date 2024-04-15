// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBNameWidget.h"
#include "Components/TextBlock.h"

UDBNameWidget::UDBNameWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBNameWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBNameWidget::SetNameText(FText InName)
{
	if (Text_Name)
	{
		Text_Name->SetText(InName);
	}
}


