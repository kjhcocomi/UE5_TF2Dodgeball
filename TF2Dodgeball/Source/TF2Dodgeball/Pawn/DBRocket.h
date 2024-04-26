// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/DBEnums.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DBRocket.generated.h"

class ADBCharacter;

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

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void FindTargetPlayer();
	void Explode(class ADBCharacter* HittedCharacter);
	void Reflect_Ready(class ADBCharacter* InAttacker);
	void Reflect();
	//void SetCurrentDirection(FVector Direction);

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> RocketBlueParticle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> RocketRedParticle;

public:
	TObjectPtr<class ADBCharacter> Attacker = nullptr;
	
	bool bReady = false;
	FTimerHandle TimerHandle_Reflect;
	float CheckTime = 0.1f;
	int ReflectCnt = 0;

private:
	UPROPERTY(EditAnywhere, Category = SphereComponent)
	TObjectPtr<class USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, Category = Movement)
	TObjectPtr<class UFloatingPawnMovement> FloaingPawnMovement;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class ADBCharacter> TargetCharacter;

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_RocketParticle)
	TeamColor AttackerTeam = TeamColor::None;

	UPROPERTY(Replicated)
	bool bFindTarget = false;

	UPROPERTY(Replicated)
	FVector CurrentDirection = FVector::UpVector;

	UFUNCTION()
	virtual void OnRep_RocketParticle();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCExplodeRocket(ADBCharacter* InAttacker, ADBCharacter* InVictim, float InRocketSpeed, float InReflectCnt);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCRocketSpeed(float RocketSpeed);
};
