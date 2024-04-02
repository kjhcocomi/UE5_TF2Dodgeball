// Fill out your copyright notice in the Description page of Project Settings.


#include "System/DBGameInstance.h"
#include "Pawn/DBRocket.h"

UDBGameInstance::UDBGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UDBGameInstance::Init()
{
	Super::Init();
}

void UDBGameInstance::Shutdown()
{
	Super::Shutdown();
}