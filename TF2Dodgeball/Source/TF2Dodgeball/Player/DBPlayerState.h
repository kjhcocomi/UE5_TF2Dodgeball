// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Utils/DBEnums.h"
#include "DBPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API ADBPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerTeamColor)
	TeamColor PlayerTeamColor;

	UPROPERTY(ReplicatedUsing = OnRep_DBCharacterState)
	DBCharacterState DBCharacterState;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Client에서 호출
	virtual void OnRep_PlayerName() override;
	virtual void OnRep_Score() override;

	UFUNCTION()
	void OnRep_PlayerTeamColor();

	UFUNCTION()
	void OnRep_DBCharacterState();
};
