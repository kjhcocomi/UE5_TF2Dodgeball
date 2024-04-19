// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/DBCharacter.h"
#include "DBPlayer.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API ADBPlayer : public ADBCharacter
{
	GENERATED_BODY()
	
public:
	ADBPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 서버에서 호출
	virtual void PossessedBy(AController* NewController) override;

	// 클라이언트에서 호출, 오너값이 동기화
	virtual void OnRep_Owner() override;
	virtual void PostNetInit() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetThirdPersonView();
	void SetFirstPersonView();

protected:
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USpringArmComponent> SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCameraComponent> Camera;

};
