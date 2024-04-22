// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBChatWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UI/DBChatElementWidget.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "Character/DBCharacter.h"
#include "Player/DBPlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UDBChatWidget::UDBChatWidget(const FObjectInitializer& ObjectInitializer)
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

void UDBChatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	EditableText_Chat->OnTextCommitted.AddDynamic(this, &ThisClass::DelegateCommitText);
}

void UDBChatWidget::ShowUI()
{
	Super::ShowUI();
	EditableText_Chat->SetFocus();
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (DBPC)
	{
		DBPC->SetInputMode(FInputModeGameAndUI());
		DBPC->ChatUIVisible = true;
	}
}

void UDBChatWidget::HideUI()
{
	Super::HideUI();
	EditableText_Chat->SetText(FText::GetEmpty());
	//UWidgetBlueprintLibrary::SetFocusToGameViewport();
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (DBPC)
	{
		DBPC->SetInputMode(FInputModeGameOnly());
		DBPC->ChatUIVisible = false;
	}
}

void UDBChatWidget::DelegateCommitText(const FText& Text, ETextCommit::Type CommitMethod)
{
	// Test
	if (CommitMethod == ETextCommit::OnEnter)
	{
		ADBPlayerController* DBPC = Cast<ADBPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (DBPC)
		{
			if (Text.IsEmpty() == false)
			{
				DBPC->ServerRPCSendMessage(Text);
			}
		}
		HideUI();
	}
}

void UDBChatWidget::AddChat(ADBCharacter* Sender, const FText& ChatText)
{
	UDBChatElementWidget* ElementWidget = CreateWidget<UDBChatElementWidget>(GetWorld(), ChatElementClass);
	ElementWidget->SetElement(Sender, ChatText);
	ScrollBox_Chat->AddChild(ElementWidget);
	ScrollBox_Chat->ScrollToEnd();
}
