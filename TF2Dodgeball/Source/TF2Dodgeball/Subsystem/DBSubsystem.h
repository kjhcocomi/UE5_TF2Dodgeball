// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DBSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:
	const FString& GetPlayerName() { return PlayerName; }
	void SetPlayerName(FString InName) { PlayerName = InName; }

protected:
	UPROPERTY()
	FString PlayerName;
};
