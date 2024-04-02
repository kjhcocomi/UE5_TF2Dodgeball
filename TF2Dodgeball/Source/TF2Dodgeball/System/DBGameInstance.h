// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/DBEnums.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UDBGameInstance(const FObjectInitializer& ObjectInitializer);

public:
	virtual void Init() override;
	virtual void Shutdown() override;
};
