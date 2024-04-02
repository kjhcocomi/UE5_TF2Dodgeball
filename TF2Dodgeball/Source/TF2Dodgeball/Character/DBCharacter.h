// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/DBEnums.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DBCharacter.generated.h"

UCLASS()
class TF2DODGEBALL_API ADBCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADBCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	const TeamColor& GetTeamColor() { return DBTeamColor; }

private:
	UFUNCTION()
	void OnBeginOverlapBody(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void AirBlast();

protected:
	UPROPERTY(EditAnywhere)
	TeamColor DBTeamColor;

};
