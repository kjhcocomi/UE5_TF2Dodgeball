// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/DBEnums.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DBRocket.generated.h"

UCLASS()
class TF2DODGEBALL_API ADBRocket : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADBRocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void FindTargetPlayer();

private:
	UPROPERTY(EditAnywhere, Category = Movement)
	TObjectPtr<class UFloatingPawnMovement> FloaingPawnMovement;

private:
	UPROPERTY()
	TObjectPtr<class ADBCharacter> TargetCharacter;
};
