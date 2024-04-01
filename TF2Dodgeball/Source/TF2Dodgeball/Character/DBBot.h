// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DBCharacter.h"
#include "DBBot.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API ADBBot : public ADBCharacter
{
	GENERATED_BODY()
	
public:
	ADBBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
