// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/DBGameInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DBGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API ADBGameModeBase : public AGameModeBase, public IDBGameInterface
{
	GENERATED_BODY()

public:
	ADBGameModeBase();

	virtual void ChangeTargetTeam() override;
	virtual TeamColor GetCurrentTargetTeam() override;
	virtual void SpawnRocket() override;

public:

	UFUNCTION()
	void OnRocketDestroyed(AActor* DestroyedActor);

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	TeamColor CurrentTargetTeam = TeamColor::Blue;

	UPROPERTY(EditAnywhere, Category = Rocket, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADBRocket> RocketClass;
};
