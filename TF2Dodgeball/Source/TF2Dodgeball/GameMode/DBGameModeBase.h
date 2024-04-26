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

public:

	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	virtual void ChangeRocketOwnerTeam() override;
	virtual TeamColor GetRocketOwnerTeam() override;
	virtual void SpawnRocket() override;

public:
	UFUNCTION()
	void OnRocketDestroyed(AActor* DestroyedActor);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	TeamColor RocketOwnerTeam = TeamColor::Blue;

	UPROPERTY(EditAnywhere, Category = Rocket, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADBRocket> RocketClass;

private:
	//UPROPERTY(VisibleAnywhere)
	//EDBGameState CurrGameState = EDBGameState::Wait;
	TObjectPtr<class ADBGameState> DodgeBallGameState;

private:
	bool bRocketValid;
	FTimerHandle TimerHandle_Ready;
	FTimerHandle TimerHandle_Finish;
	TObjectPtr<class ADBRocket> DBRocket;

	TArray<class AController*> DBControllers;
	TArray<class ADBCharacter*> DBCharacters;
	TArray<class ADBCharacter*> DBBlueCharacters;
	TArray<class ADBCharacter*> DBRedCharacters;

private:
	void GatherCharacterInfo();
	void Wait();
	void Ready();
	void Progress();
	void Finish();

	void ReadyToProgress();
	void FinishToReady();

public:
	const TArray<class ADBCharacter*>& GetCharacters();
	const TArray<class ADBCharacter*>& GetBlueCharacters();
	const TArray<class ADBCharacter*>& GetRedCharacters();
};
