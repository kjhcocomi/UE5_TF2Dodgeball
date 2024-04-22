// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBChatElementWidget.h"
#include "Character/DBCharacter.h"
#include "Components/RichTextBlock.h"
#include "Utils/DBEnums.h"

UDBChatElementWidget::UDBChatElementWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBChatElementWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDBChatElementWidget::SetElement(ADBCharacter* Sender, const FText& ChatText, bool IsTmp)
{
	if (IsTmp)
	{
		GetWorld()->GetTimerManager().SetTimer(RemoveHandle, this, &UDBChatElementWidget::Remove, 5.f, false);
	}
	FString SenderName = FString::Printf(TEXT(""));
	FString ChatTextLocal = FString::Printf(TEXT(""));
	
	// Sender
	if (Sender)
	{
		DBCharacterState CharacterStateLocal = Sender->GetCharacterState();
		if (CharacterStateLocal == DBCharacterState::Dead)
		{
			SenderName += FString::Printf(TEXT("*Dead* "));
		}
		else if (CharacterStateLocal == DBCharacterState::Spectate)
		{
			SenderName += FString::Printf(TEXT("*Dead* "));
		}

		TeamColor TeamColorLocal = Sender->PlayerTeamColor;
		if (TeamColorLocal == TeamColor::Blue)
		{
			SenderName += FString::Printf(TEXT("<Blue>")) + Sender->PlayerName + FString::Printf(TEXT("</>"));
		}
		else if (TeamColorLocal == TeamColor::Red)
		{
			SenderName += FString::Printf(TEXT("<Red>")) + Sender->PlayerName + FString::Printf(TEXT("</>"));
		}
		else if (TeamColorLocal == TeamColor::Spectate)
		{
			SenderName += FString::Printf(TEXT("<Spec>")) + Sender->PlayerName + FString::Printf(TEXT("</>"));
		}
	}
	else
	{
		SenderName = FString::Printf(TEXT("System"));
	}

	// Chat
	if (ChatText.IsEmpty() == false)
	{
		ChatTextLocal = ChatText.ToString();
	}

	// Total Chat
	FString TotalChat = SenderName + FString::Printf(TEXT(" : ")) + ChatTextLocal;

	RichTextBlock_Chat->SetText(FText::FromString(TotalChat));
}

void UDBChatElementWidget::Remove()
{
	RemoveFromParent();
}
