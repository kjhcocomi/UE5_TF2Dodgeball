// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Utils/DBEnums.h"
#include "DBGameState.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API ADBGameState : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	//virtual void HandleBeginPlay() override;
	//virtual void OnRep_ReplicatedHasBegunPlay() override;

	UPROPERTY(Replicated)
	EDBGameState CurrentDBGameState;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void SetCurrentGameState(EDBGameState GS);
	EDBGameState GetCurrentGameState() { return CurrentDBGameState; }
};