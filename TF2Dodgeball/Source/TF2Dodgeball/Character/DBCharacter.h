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
	TeamColor GetTeamColor() { return PlayerTeamColor; }

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
	void Revive();
	void Spectate();
	void StartGame();
	void OnDamaged(class ADBRocket* DBRocket);

	DBCharacterState GetCharacterState();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Weapon;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UWidgetComponent> NameTextComponent;

public:
	void SetName(FText InText);

public:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerTeamColor, EditAnywhere)
	TeamColor PlayerTeamColor;

	UPROPERTY(ReplicatedUsing = OnRep_DBCharacterState)
	DBCharacterState DBCharacterStateLocal;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	// Client���� ȣ��

	UFUNCTION()
	void OnRep_PlayerTeamColor();

	UFUNCTION()
	void OnRep_DBCharacterState();

	// ServerRPC
	UFUNCTION(Server, Reliable)
	void ServerRPCSetTeam(TeamColor InTeamColor);

	UFUNCTION(Server, Reliable)
	void ServerRPCSetState(DBCharacterState InState);
};
