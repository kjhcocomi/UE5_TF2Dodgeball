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

public:
	const TeamColor& GetCurrentTargetTeam();
	void ChangeTargetTeam();
	void SpawnRocket();
	// TODO
	void Enter();
	void Exit();

public:

	UPROPERTY()
	TeamColor CurrentTargetTeam = TeamColor::Red;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APawn> RocketBlueprint;

	UPROPERTY()
	FVector RocketSpawnLocation = FVector::UpVector * 5.f;
};
