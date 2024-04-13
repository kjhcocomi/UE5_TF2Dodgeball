// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DBBot.h"
#include "Net/UnrealNetwork.h"
#include "TF2Dodgeball.h"

ADBBot::ADBBot()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	bReplicates = true;
	NetUpdateFrequency = 1.0f;
}

void ADBBot::BeginPlay()
{
	Super::BeginPlay();
	// tmp
	DBTeamColor = TeamColor::Red;
}

void ADBBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (HasAuthority())
	{
		AddActorLocalRotation(FRotator(0.f, RotationRate * DeltaTime, 0.f));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
	}
	else
	{
		//FRotator NewRotator = RootComponent->GetComponentRotation();
		//NewRotator.Yaw = ServerRotationYaw;
		//RootComponent->SetWorldRotation(NewRotator);
	}
}

void ADBBot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADBBot, ServerRotationYaw);
}

void ADBBot::OnRep_ServerRotationYaw()
{
	DB_LOG(LogDBNetwork, Log, TEXT("%s"), TEXT("OnRep_ServerRotationYaw"));
}
