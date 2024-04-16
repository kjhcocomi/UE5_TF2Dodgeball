// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DBUIManagerSubsystem.generated.h"

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

public:
	void ShowSelectTeamUI();
};
