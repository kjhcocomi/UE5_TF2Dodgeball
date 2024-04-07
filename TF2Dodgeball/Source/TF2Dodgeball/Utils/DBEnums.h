// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM()
enum class TeamColor 
{
	None,
	Red,
	Blue
};

UENUM()
enum class DBCharacterState
{
	Dead,
	Wait,
	Alive,
	Spectate,
};

UENUM()
enum class DBGameState
{
	Wait,
	Ready,
	Progress,
	Finish,
};