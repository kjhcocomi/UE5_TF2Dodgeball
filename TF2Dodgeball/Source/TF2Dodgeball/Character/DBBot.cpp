// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DBBot.h"

ADBBot::ADBBot()
{
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

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
}
