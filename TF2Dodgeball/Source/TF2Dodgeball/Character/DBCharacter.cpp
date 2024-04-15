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
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBeginOverlapBody);
}

void ADBCharacter::OnBeginOverlapBody(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADBRocket* DBRocket = Cast<ADBRocket>(OtherActor);
	if (DBRocket)
	{
		if (DBRocket->Owner == this) return;
		DBRocket->Explode(this);
	}
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
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 80.f;
	const float AttackRadius = 80.f;
	const float AttackDamage = 80.f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_DBAIRBLAST, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		ADBRocket* DBRocket = Cast<ADBRocket>(OutHitResult.GetActor());
		if (DBRocket && DBTeamColor != DBRocket->AttackerTeam)
		{
			DBRocket->SetCurrentDirection(GetActorForwardVector());
			DBRocket->Reflect(this);
		}
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 5.0f);

#endif
}

void ADBCharacter::Revive()
{
	SetActorHiddenInGame(false);
	ADBPlayerState* DBPS = Cast<ADBPlayerState>(GetPlayerState());
	if (DBPS)
	{
		DBPS->DBCharacterState = DBCharacterState::Ready;
	}
	// TODO : 캐릭터 부활, 움직이지는 못함
}

void ADBCharacter::Spectate()
{
	ADBPlayerState* DBPS = Cast<ADBPlayerState>(GetPlayerState());
	if (DBPS)
	{
		DBPS->DBCharacterState = DBCharacterState::Spectate;
	}
}

void ADBCharacter::StartGame()
{
	ADBPlayerState* DBPS = Cast<ADBPlayerState>(GetPlayerState());
	if (DBPS)
	{
		DBPS->DBCharacterState = DBCharacterState::Alive;
	}
}

void ADBCharacter::OnDamaged(ADBRocket* DBRocket)
{
	SetActorHiddenInGame(true);
	ADBPlayerState* DBPS = Cast<ADBPlayerState>(GetPlayerState());
	if (DBPS)
	{
		DBPS->DBCharacterState = DBCharacterState::Spectate;
	}
	
}

DBCharacterState ADBCharacter::GetCharacterState()
{
	ADBPlayerState* DBPS = Cast<ADBPlayerState>(GetPlayerState());
	if (DBPS)
	{
		return DBPS->DBCharacterState;
	}
	return DBCharacterState::None;
}

void ADBCharacter::SetName(FText InText)
{
	if (NameTextComponent)
	{
		UDBNameWidget* NameWidget = Cast<UDBNameWidget>(NameTextComponent->GetUserWidgetObject());
		if (NameWidget)
		{
			NameWidget->SetNameText(InText);
			DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("Good"));
		}
	}
}

