﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/DBRocket.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Character/DBPlayer.h"
#include "System/DBGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/DBGameInterface.h"
#include "GameMode/DBGameModeBase.h"
#include "Player/DBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBKillLogWidget.h"
#include "GameMode/DBGameState.h"
#include "Particles/ParticleSystemComponent.h"
#include "UI/DBHudWidget.h"
#include "UI/DBChatWidget.h"
#include "UI/DBChatTmpWidget.h"


// Sets default values
ADBRocket::ADBRocket()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(SphereComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Tf2/Rocket/Rocket.Rocket'")
	);

	if (SphereMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(SphereMesh.Object);
	}

	FloaingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloaingPawnMovement"));
	bReplicates = true;
	NetUpdateFrequency = 300;
}

// Called when the game starts or when spawned
void ADBRocket::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlap);
		MulticastRPCRocketSpeed(FloaingPawnMovement->MaxSpeed);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Tick, this, &ADBRocket::CustomTick, 0.05f, true, 0.f);
	}
}

// Called every frame
void ADBRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		
	}
	else
	{
		if (bFindTarget)
		{
			//UE_LOG(LogTemp, Log, TEXT("Find %s"), *CurrentDirection.ToString());
			//AddMovementInput(CurrentDirection, 1);
		}
	}
}

// Called to bind functionality to input
void ADBRocket::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADBRocket::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADBCharacter* DBCharacter = Cast<ADBCharacter>(OtherActor);
	if (DBCharacter)
	{
		if (DBCharacter != Attacker)
		{
			Explode(DBCharacter);
		}
	}
}

void ADBRocket::FindTargetPlayer()
{
	ADBGameModeBase* DBGameMode = Cast<ADBGameModeBase>(GetWorld()->GetAuthGameMode());
	TeamColor TargetTeam;
	TArray<ADBCharacter*> DBTargetCharacters;
	// 타겟 팀 변경, 타겟 캐릭터들 얻어옴
	if (AttackerTeam == TeamColor::None) 
	{
		// 서브공은 Owner에게 
		TargetTeam = DBGameMode->GetRocketOwnerTeam();
		if (TargetTeam == TeamColor::Blue)
		{
			AttackerTeam = TeamColor::Red;
			DBTargetCharacters = DBGameMode->GetBlueCharacters();
		}
		else
		{
			AttackerTeam = TeamColor::Blue;
			DBTargetCharacters = DBGameMode->GetRedCharacters();
		}
	}
	else if (AttackerTeam == TeamColor::Blue)
	{
		TargetTeam = TeamColor::Red;
		DBTargetCharacters = DBGameMode->GetRedCharacters();
	}
	else
	{
		TargetTeam = TeamColor::Blue;
		DBTargetCharacters = DBGameMode->GetBlueCharacters();
	}
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, FString::Printf(TEXT("%d"), DBTargetCharacters.Num()));

	// 타겟 캐릭터들 중 살아있는 캐릭터만 추출
	TArray<ADBCharacter*> AliveTargetCharacters;
	for (int i = 0; i < DBTargetCharacters.Num(); i++)
	{
		if (DBTargetCharacters[i]->GetCharacterState() == DBCharacterState::Alive)
		{
			AliveTargetCharacters.Add(DBTargetCharacters[i]);
		}
	}

	// tmp
	if (AliveTargetCharacters.IsEmpty() == false)
	{
		ADBCharacter* TargetTmp = nullptr;
		if (Attacker)
		{
			float MaxDotValue = -999999999.f;
			for (int i = 0; i < AliveTargetCharacters.Num(); i++)
			{
				FVector RocketDir = CurrentDirection;
				FVector RocketToTargetVec = AliveTargetCharacters[i]->GetActorLocation() - Attacker->GetActorLocation();
				float DotValue = FVector::DotProduct(RocketDir, RocketToTargetVec);
				if (DotValue > MaxDotValue)
				{
					MaxDotValue = DotValue;
					TargetTmp = AliveTargetCharacters[i];
				}
			}
		}
		else
		{
			int RandVal = UKismetMathLibrary::RandomInteger(AliveTargetCharacters.Num());
			TargetTmp = AliveTargetCharacters[RandVal];
		}
		TargetCharacter = TargetTmp;
		if (TargetCharacter)
		{
			TargetCharacter->ClientRPCBeepSound();
		}
	}
	else
	{
		TargetCharacter = nullptr;
	}
}

void ADBRocket::Explode(ADBCharacter* HittedCharacter)
{
	ADBGameModeBase* DBGameMode = Cast<ADBGameModeBase>(GetWorld()->GetAuthGameMode());
	if (DBGameMode)
	{
		// tmp
		if (AttackerTeam != HittedCharacter->GetTeamColor())
		{
			HittedCharacter->OnDamaged(this);
			if (Attacker)
			{
				Attacker->KillCount++;
				Attacker->ClientRPCHitSucceed();
			}
			MulticastRPCExplodeRocket(Attacker, HittedCharacter, FloaingPawnMovement->MaxSpeed, ReflectCnt);
			MulticastRPCRocketSpeed(-1.f);
			// TODO : ClientRPC Damage Sound, Damage UI
			if (DBGameMode->GetRocketOwnerTeam() != AttackerTeam)
			{
				DBGameMode->ChangeRocketOwnerTeam();
			}
		}
		// TODO : 폭발
		Destroy();
	}
}

void ADBRocket::Reflect_Ready(ADBCharacter* InAttacker)
{
	if (bReady) return;
	bReady = true;
	Attacker = InAttacker;
	AttackerTeam = Attacker->GetTeamColor();
	CurrentDirection = FVector::ZeroVector;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Reflect, this, &ADBRocket::Reflect, 0.05f, false);
}

void ADBRocket::Reflect()
{
	ReflectCnt++;
	FloaingPawnMovement->MaxSpeed = FloaingPawnMovement->MaxSpeed + 180.f;
	MulticastRPCRocketSpeed(FloaingPawnMovement->MaxSpeed);

	FVector Direction = Attacker->GetController()->GetControlRotation().Vector();
	Direction.Normalize();
	CurrentDirection = Direction;

	FindTargetPlayer();
	bReady = false;
}

void ADBRocket::CustomTick()
{
	if (HasAuthority())
	{
		if (TargetCharacter != nullptr)
		{
			bFindTarget = true;

			FVector TargetPosition = TargetCharacter->GetController()->GetPawn()->GetActorLocation();
			FVector MyPosition = GetActorLocation();

			FVector TargetDirection = TargetPosition - MyPosition;
			TargetDirection.Normalize();

			float t = TurnRate + TurnRateInc * ReflectCnt;
			if (t < 0.f) t = 0.f;
			if (t > 1.f) t = 1.f;

			

			double Distance = FVector::Distance(TargetPosition, MyPosition);
			if (Distance >= 300.f)
			{
				//CurrentDirection = CurrentDirection + TargetDirection * DeltaTime * FloaingPawnMovement->MaxSpeed / 260.f;
				CurrentDirection = CurrentDirection + (TargetDirection - CurrentDirection) * t / 3;
				TmpValue = 1.5f;
			}
			else
			{
				TmpValue += 0.05f;
				if (TmpValue > 3) TmpValue = 3;
				//CurrentDirection = CurrentDirection + TargetDirection * DeltaTime * FloaingPawnMovement->MaxSpeed / 70.f;
				CurrentDirection = CurrentDirection + (TargetDirection - CurrentDirection) * t / 3 * TmpValue;
			}

			CurrentDirection.Normalize();
			AddMovementInput(CurrentDirection, 1);
			SetActorRotation(CurrentDirection.Rotation());
			/*UPrimitiveComponent* UPC = Cast<UPrimitiveComponent>(GetRootComponent());
			if (UPC)
			{
				UPC->SetPhysicsLinearVelocity(CurrentDirection * FloaingPawnMovement->MaxSpeed);
			}*/
		}
		else
		{
			bFindTarget = false;
			//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Cyan, TEXT("No Target"));
			FindTargetPlayer();
		}
	}
}

//void ADBRocket::SetCurrentDirection(FVector Direction)
//{
//	Direction.Normalize();
//	CurrentDirection = Direction;
//}

void ADBRocket::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBRocket, bFindTarget);
	DOREPLIFETIME(ADBRocket, CurrentDirection);
	DOREPLIFETIME(ADBRocket, AttackerTeam);
}

void ADBRocket::OnRep_RocketParticle()
{
	UParticleSystemComponent* PS = Cast<UParticleSystemComponent>(GetComponentByClass(UParticleSystemComponent::StaticClass()));
	if (PS)
	{
		if (AttackerTeam == TeamColor::Blue)
		{
			PS->SetTemplate(RocketBlueParticle);
		}
		else if (AttackerTeam == TeamColor::Red)
		{
			PS->SetTemplate(RocketRedParticle);
		}
	}
}

void ADBRocket::MulticastRPCExplodeRocket_Implementation(ADBCharacter* InAttacker, ADBCharacter* InVictim, float InRocketSpeed, float InReflectCnt)
{
	if (HasAuthority() == false)
	{
		UDBUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UDBUIManagerSubsystem>();
		if (UIManager)
		{
			UDBKillLogWidget* KillLogUI = UIManager->GetKillLogWidget();
			if (KillLogUI)
			{
				KillLogUI->AddKillLogElement(InAttacker, InVictim);
			}
			FString VictimName = InVictim->PlayerName;
			int RocketSpeed = (int)(InRocketSpeed) / 20;
			FString DeadMessage = VictimName + FString::Printf(TEXT(" 님이 사망하였습니다. (Speed : "))
				+ FString::FromInt(RocketSpeed) + FString::Printf(TEXT(", Deflection : "))
				+ FString::FromInt(InReflectCnt) + FString::Printf(TEXT(")"));

			UDBChatWidget* ChatUI = UIManager->GetChatWidget();
			if (ChatUI)
			{
				ChatUI->AddChat(nullptr, FText::FromString(DeadMessage));
			}
			UDBChatTmpWidget* ChatTmpUI = UIManager->GetChatTmpWidget();
			if (ChatTmpUI)
			{
				ChatTmpUI->AddChat(nullptr, FText::FromString(DeadMessage));
			}
		}
		// effect
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation());

	}
}

void ADBRocket::MulticastRPCRocketSpeed_Implementation(float RocketSpeed)
{
	if (HasAuthority() == false)
	{
		UDBUIManagerSubsystem* UIManager = GetGameInstance()->GetSubsystem<UDBUIManagerSubsystem>();
		if (UIManager)
		{
			UDBHudWidget* Hud = UIManager->GetHudUI();
			if (Hud)
			{
				Hud->SetRocketSpeedText(RocketSpeed);
			}
		}
	}
}