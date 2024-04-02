// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/DBEnums.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DBGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDBGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TF2DODGEBALL_API IDBGameInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ChangeTargetTeam() = 0;
	virtual TeamColor GetCurrentTargetTeam() = 0;
	virtual void SpawnRocket() = 0;
};
