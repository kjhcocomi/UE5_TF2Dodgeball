// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawn/DBRocket.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Character/DBPlayer.h"
#include "System/DBGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/DBGameInterface.h"
#include "GameMode/DBGameModeBase.h"
#include "Player/DBPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Subsystem/DBUIManagerSubsystem.h"
#include "UI/DBKillLogWidget.h"
#include "GameMode/DBGameState.h"

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
	}
}

// Called every frame
void ADBRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		if (TargetCharacter != nullptr)
		{
			bFindTarget = true;
			
			FVector TargetPosition = TargetCharacter->GetController()->GetPawn()->GetActorLocation();
			FVector MyPosition = GetActorLocation();

			FVector TargetDirection = TargetPosition - MyPosition;
			TargetDirection.Normalize();

			double Distance = FVector::Distance(TargetPosition, MyPosition);
			if (Distance >= 300.f)
			{
				CurrentDirection = CurrentDirection + TargetDirection * DeltaTime * FloaingPawnMovement->MaxSpeed / 240.f;
			}
			else
			{
				CurrentDirection = CurrentDirection + TargetDirection * DeltaTime * FloaingPawnMovement->MaxSpeed / 80.f;
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
		if (TargetTeam == TeamColor::Blue) DBTargetCharacters = DBGameMode->GetBlueCharacters();
		else DBTargetCharacters = DBGameMode->GetRedCharacters();
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
		TargetCharacter = AliveTargetCharacters[0];
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
			if (Attacker) Attacker->KillCount++;
			MulticastRPCExplodeRocket(Attacker, HittedCharacter);
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
	UE_LOG(LogTemp, Log, TEXT("Ready"));
	bReady = true;
	Attacker = InAttacker;
	AttackerTeam = Attacker->GetTeamColor();
	CurrentDirection = FVector::ZeroVector;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Reflect, this, &ADBRocket::Reflect, 0.05f, false);
}

void ADBRocket::Reflect()
{
	
	FloaingPawnMovement->MaxSpeed = FloaingPawnMovement->MaxSpeed + 180.f;

	FVector Direction = Attacker->GetController()->GetControlRotation().Vector();
	Direction.Normalize();
	CurrentDirection = Direction;

	UE_LOG(LogTemp, Log, TEXT("%s"), *Direction.ToString());

	FindTargetPlayer();
	bReady = false;
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
}

void ADBRocket::MulticastRPCExplodeRocket_Implementation(ADBCharacter* InAttacker, ADBCharacter* InVictim)
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
		}
	}
}
