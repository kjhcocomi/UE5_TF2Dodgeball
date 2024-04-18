// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DBUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void ShowUI();
	virtual void HideUI();
	
};
