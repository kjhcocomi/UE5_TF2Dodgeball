// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Utils/DBEnums.h"
#include "Kismet/GameplayStatics.h"
#include "DBGameState.generated.h"

class ADBCharacter;
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
	UPROPERTY(Replicated)
	int BlueWinCount = 0;

	UPROPERTY(Replicated)
	int RedWinCount = 0;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CountDownSound5;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CountDownSound4;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CountDownSound3;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CountDownSound2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CountDownSound1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> CountDownSound0;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> VictorySound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoseSound;

	FTimerHandle TimerHandle5;
	FTimerHandle TimerHandle4;
	FTimerHandle TimerHandle3;
	FTimerHandle TimerHandle2;
	FTimerHandle TimerHandle1;
	FTimerHandle TimerHandle0;

	void Count5();
	void Count4();
	void Count3();
	void Count2();
	void Count1();
	void Count0();

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void SetCurrentGameState(EDBGameState GS);
	EDBGameState GetCurrentGameState() { return CurrentDBGameState; }

public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadCastMessage(class ADBCharacter* Sender, const FText& Text);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCReady(int second);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCReadyCancel();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCGameResult(TeamColor WinnerTeam);
};
