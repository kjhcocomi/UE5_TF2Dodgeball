// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBChatTmpWidget.h"
#include "UI/DBChatElementWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/VerticalBox.h"
#include "Character/DBCharacter.h"

UDBChatTmpWidget::UDBChatTmpWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UDBChatElementWidget> ChatElementWidgetClass(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_ChatElement.WBP_ChatElement_C'")
	);

	if (ChatElementWidgetClass.Succeeded())
	{
		ChatElementClass = ChatElementWidgetClass.Class;
	}
}

void UDBChatTmpWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBChatTmpWidget::ShowUI()
{
	Super::ShowUI();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = false;
}

void UDBChatTmpWidget::AddChat(ADBCharacter* Sender, const FText& ChatText)
{
	UDBChatElementWidget* ElementWidget = CreateWidget<UDBChatElementWidget>(GetWorld(), ChatElementClass);
	ElementWidget->SetElement(Sender, ChatText, true);
	VerticalBox_ChatTmp->AddChildToVerticalBox(ElementWidget);
}
