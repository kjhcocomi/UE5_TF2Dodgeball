// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/LobbyGameMode.h"
#include "UI/DBLobbyWidget.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	static ConstructorHelpers::FClassFinder<UDBLobbyWidget> LobbyWidgetAsset(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_Lobby.WBP_Lobby_C'")
	);

	if (LobbyWidgetAsset.Succeeded())
	{
		LobbyWidgetClass = LobbyWidgetAsset.Class;
	}
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(LobbyWidgetClass))
	{
		LobbyWidget = CreateWidget<UDBLobbyWidget>(GetWorld(), LobbyWidgetClass);
		if (IsValid(LobbyWidget))
		{
			LobbyWidget->AddToViewport();
			LobbyWidget->ShowUI();
		}
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LobbyBGMSound, FVector(0,0,0));

}
