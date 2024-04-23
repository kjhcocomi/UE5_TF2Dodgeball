// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DBUIManagerSubsystem.generated.h"

class ADBCharacter;
class UDBKillLogWidget;
class UDBChatWidget;
class UDBChatTmpWidget;
class UDBHudWidget;
/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDBUIManagerSubsystem();

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:

	UPROPERTY()
	TSubclassOf<class UDBSelectTeamWidget> SelectTeamWidgetClass;
	UPROPERTY()
	TObjectPtr<class UDBSelectTeamWidget> SelectTeamWidget;

	UPROPERTY()
	TSubclassOf<class UDBScoreBoardWidget> ScoreBoardWidgetClass;
	UPROPERTY()
	TObjectPtr<class UDBScoreBoardWidget> ScoreBoardWidget;

	UPROPERTY()
	TSubclassOf<UDBKillLogWidget> KillLogWidgetClass;
	UPROPERTY()
	TObjectPtr<UDBKillLogWidget> KillLogWidget;

	UPROPERTY()
	TSubclassOf<UDBChatWidget> ChatWidgetClass;
	UPROPERTY()
	TObjectPtr<UDBChatWidget> ChatWidget;

	UPROPERTY()
	TSubclassOf<UDBChatTmpWidget> ChatTmpWidgetClass;
	UPROPERTY()
	TObjectPtr<UDBChatTmpWidget> ChatTmpWidget;

	UPROPERTY()
	TSubclassOf<UDBHudWidget> HudWidgetClass;
	UPROPERTY()
	TObjectPtr<UDBHudWidget> HudWidget;

public:
	void ShowSelectTeamUI();

	void ShowScoreBoardUI();
	void HideScoreBoardUI();

	void ShowKillLogUI();

	void ShowChatUI();
	void HideChatUI();

	void ShowChatTmpUI();

	void ShowHudUI();

public:
	UDBKillLogWidget* GetKillLogWidget() { return KillLogWidget; }
	UDBChatWidget* GetChatWidget() { return ChatWidget; }
	UDBChatTmpWidget* GetChatTmpWidget() { return ChatTmpWidget; }
	UDBHudWidget* GetHudUI() { return HudWidget; }
};
