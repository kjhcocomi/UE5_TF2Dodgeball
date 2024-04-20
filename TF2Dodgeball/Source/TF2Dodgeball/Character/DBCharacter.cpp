// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DBCharacter.h"
#include "Components/CapsuleComponent.h"
#include "System/DBGameInstance.h"
#include "Interfaces/DBGameInterface.h"
#include "GameMode/DBGameModeBase.h"
#include "Pawn/DBRocket.h"
#include "Physics/DBCollision.h"
#include "Player/DBPlayerState.h"
#include "TF2Dodgeball.h"
#include "Components/WidgetComponent.h"
#include "UI/DBNameWidget.h"
#include "Net/UnrealNetwork.h"
#include "Materials/Material.h"
#include "Player/DBPlayerController.h"

// Sets default values
ADBCharacter::ADBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NameTextComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameBar"));
	NameTextComponent->SetupAttachment(GetRootComponent());

	ConstructorHelpers::FClassFinder<UUserWidget> NameBarWidgetClass(
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_NameBar.WBP_NameBar_C'")
	);
	if (NameBarWidgetClass.Succeeded())
	{
		NameTextComponent->SetWidgetClass(NameBarWidgetClass.Class);
		NameTextComponent->SetWidgetSpace(EWidgetSpace::Screen);
		NameTextComponent->SetDrawAtDesiredSize(true);
		NameTextComponent->SetRelativeLocation(FVector(0, 0, 100));
		NameTextComponent->InitWidget();
	}
}

// Called when the game starts or when spawned
void ADBCharacter::BeginPlay()
{
	Super::BeginPlay();
	//GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlapBody);
}

void ADBCharacter::OnBeginOverlapBody(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (IsLocallyControlled())
	{
		ADBRocket* DBRocket = Cast<ADBRocket>(OtherActor);
		if (DBRocket)
		{
			if (DBRocket->Owner == this) return;
			UE_LOG(LogTemp, Log, TEXT("Overlap"));
			return;
			DBRocket->Explode(this);
		}
	}*/
	
	//IDBGameInterface* DBGameInterface = Cast<IDBGameInterface>(GetWorld()->GetAuthGameMode());
	//if (DBTeamColor == DBGameInterface->GetCurrentTargetTeam())
	//{
	//	// 상대팀에게 서브공
	//	DBGameInterface->ChangeTargetTeam();
	//}
	// 로켓 폭발
	
}

// Called every frame
void ADBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADBCharacter::AirBlast()
{
	//FHitResult OutHitResult;
	//FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 300.f;
	const float AttackRadius = 100.f;
	FVector Start;
	FVector End;
	ADBPlayerController* DBPC = Cast<ADBPlayerController>(GetController());
	FRotator ControlRotation;
	if (DBPC)
	{
		ControlRotation = DBPC->GetControlRotation();
		Start = GetActorLocation() + ControlRotation.Vector() * GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.3f;
		End = Start + ControlRotation.Vector() * AttackRange;
	}
	else
	{
		Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
		End = Start + GetActorForwardVector() * AttackRange;
	}
	
	ServerRPCAirBlast(AttackRange, AttackRadius, Start, End);

#if ENABLE_DRAW_DEBUG

	if (DBPC)
	{
		FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
		float CapsuleHalfHeight = AttackRange * 0.5f;
		FColor DrawColor = FColor::Red;

		DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(ControlRotation.Vector()).ToQuat(), DrawColor, false, 5.0f);
	}
#endif
}

void ADBCharacter::Revive()
{
	DBCharacterStateLocal = DBCharacterState::Ready;
	// TODO : 캐릭터 부활, 움직이지는 못함
}

void ADBCharacter::Spectate()
{
	DBCharacterStateLocal = DBCharacterState::Spectate;
}

void ADBCharacter::StartGame()
{
	DBCharacterStateLocal = DBCharacterState::Alive;
}

void ADBCharacter::OnDamaged(ADBRocket* DBRocket)
{
	DBCharacterStateLocal = DBCharacterState::Spectate;
	DeathCount++;
}

DBCharacterState ADBCharacter::GetCharacterState()
{
	return DBCharacterStateLocal;
}

void ADBCharacter::SetName(FText InText)
{
	if (NameTextComponent)
	{
		UDBNameWidget* NameWidget = Cast<UDBNameWidget>(NameTextComponent->GetUserWidgetObject());
		if (NameWidget)
		{
			NameWidget->SetNameText(InText);
		}
	}
}

void ADBCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBCharacter, PlayerName);
	DOREPLIFETIME(ADBCharacter, PlayerTeamColor);
	DOREPLIFETIME(ADBCharacter, DBCharacterStateLocal);
	DOREPLIFETIME(ADBCharacter, KillCount);
	DOREPLIFETIME(ADBCharacter, DeathCount);
}


void ADBCharacter::OnRep_DBPlayerName()
{
	SetName(FText::FromString(PlayerName));
}

void ADBCharacter::OnRep_PlayerTeamColor()
{
	switch (PlayerTeamColor)
	{
		case TeamColor::Spectate:
			DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Spectate Team"));
			break;
		case TeamColor::Red:
		{
			if (RedMaterial)
			{
				GetMesh()->SetMaterial(0, RedMaterial);
			}
		}
			break;
		case TeamColor::Blue:
		{
			if (BlueMaterial)
			{
				GetMesh()->SetMaterial(0, BlueMaterial);
			}
		}
			break;
	}
}

void ADBCharacter::OnRep_DBCharacterState()
{
	switch (DBCharacterStateLocal)
	{
	case DBCharacterState::None:
		break;
	case DBCharacterState::Dead:
		break;
	case DBCharacterState::Ready:
		SetActorHiddenInGame(false);
		break;
	case DBCharacterState::Alive:
		break;
	case DBCharacterState::Spectate:
		SetActorHiddenInGame(true);
		break;
	}
}

void ADBCharacter::ServerRPCAirBlast_Implementation(float InAttackRange, float InAttackRadius, FVector InStart, FVector InEnd)
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, InStart, InEnd, FQuat::Identity, CCHANNEL_DBAIRBLAST, FCollisionShape::MakeSphere(InAttackRadius), Params);
	if (HitDetected)
	{
		ADBRocket* DBRocket = Cast<ADBRocket>(OutHitResult.GetActor());
		if (DBRocket && PlayerTeamColor != DBRocket->AttackerTeam)
		{
			DBRocket->Reflect_Ready(this);
		}
	}
}

void ADBCharacter::ServerRPCSetTeam_Implementation(TeamColor InTeamColor)
{
	PlayerTeamColor = InTeamColor;
	if (PlayerTeamColor == TeamColor::Spectate)
	{
		Spectate();
	}
}

void ADBCharacter::ServerRPCSetState_Implementation(DBCharacterState InState)
{
	DBCharacterStateLocal = InState;
}

void ADBCharacter::ServerRPCSetName_Implementation(const FString& InName)
{
	PlayerName = InName;
}
