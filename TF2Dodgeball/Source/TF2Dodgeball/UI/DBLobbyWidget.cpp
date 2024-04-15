// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DBLobbyWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/DBSubsystem.h"

UDBLobbyWidget::UDBLobbyWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Start->OnClicked.AddDynamic(this, &UDBLobbyWidget::PushStartButton);
	Button_Quit->OnClicked.AddDynamic(this, &UDBLobbyWidget::PushQuitButton);
}

void UDBLobbyWidget::PushStartButton()
{
	FText IPText = TextBox_IP->GetText();
	FText NameText = TextBox_Name->GetText();
	if (IPText.IsEmpty() || NameText.IsEmpty())
	{
		return;
	}
	UDBSubsystem* Ss = GetGameInstance()->GetSubsystem<UDBSubsystem>();
	Ss->SetPlayerName(NameText.ToString());
	UGameplayStatics::OpenLevel(GetWorld(), FName(*IPText.ToString()));
}

void UDBLobbyWidget::PushQuitButton()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
