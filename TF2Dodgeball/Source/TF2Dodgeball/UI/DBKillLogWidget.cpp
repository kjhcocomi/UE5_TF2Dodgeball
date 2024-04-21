// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBKillLogWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DBKillLogElementWidget.h"
#include "Character/DBCharacter.h"
#include "Components/VerticalBox.h"

UDBKillLogWidget::UDBKillLogWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UDBKillLogElementWidget> KillLogElementWidgetClass(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_KillLogElement.WBP_KillLogElement_C'")
	);

	if (KillLogElementWidgetClass.Succeeded())
	{
		KillLogElementClass = KillLogElementWidgetClass.Class;
	}
}

void UDBKillLogWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBKillLogWidget::ShowUI()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
}

void UDBKillLogWidget::AddKillLogElement(ADBCharacter* InAttacker, ADBCharacter* InVictim)
{
	UDBKillLogElementWidget* ElementWidget = CreateWidget<UDBKillLogElementWidget>(GetWorld(), KillLogElementClass);
	ElementWidget->SetElement(InAttacker, InVictim);
	VerticalBox_KillLog->AddChildToVerticalBox(ElementWidget);
}
