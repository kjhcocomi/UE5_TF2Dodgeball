// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();
	virtual void BeginPlay() override;

public:

	UPROPERTY()
	TSubclassOf<class UDBLobbyWidget> LobbyWidgetClass;

	UPROPERTY()
	TObjectPtr<class UDBLobbyWidget> LobbyWidget;
};
