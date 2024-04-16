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
	ADBPlayerState();

public:
	virtual void BeginPlay() override;
};
