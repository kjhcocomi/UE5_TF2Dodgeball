// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class TeamColor 
{
	None,
	Red,
	Blue,
	Spectate,
};

UENUM()
enum class DBCharacterState
{
	None,
	Dead,
	Ready,
	Alive,
	Spectate,
};

UENUM()
enum class EDBGameState
{
	None,
	Wait,
	Ready,
	Progress,
	Finish,
};