// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DBPlayerController.generated.h"

struct FInputActionValue;
/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API ADBPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADBPlayerController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* InPawn) override;

private:
	void Move(const FInputActionValue& InputValue);
	void Turn(const FInputActionValue& InputValue);
	void Jump(const FInputActionValue& InputValue);
	void StopJump(const FInputActionValue& InputValue);
	void AirBlast(const FInputActionValue& InputValue);
	void ShowScore(const FInputActionValue& InputValue);
	void HideScore(const FInputActionValue& InputValue);
	void ShowSelectTeamUI(const FInputActionValue& InputValue);
	void ShowMenuUI(const FInputActionValue& InputValue);

	void CoolDown_AirBlast();

private:
	FTimerHandle TimerHandle_AirBlast;
	bool bCanAirBlast = true;

protected:
	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> TurnAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> AirBlastAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ShowScoreBoardAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ShowSelectTeamUIAction;

	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<class UInputAction> ShowMenuUIAction;

protected:
	UPROPERTY(EditAnywhere, Category = Anim)
	TObjectPtr<class UAnimMontage> AirBlastMontage;
};
