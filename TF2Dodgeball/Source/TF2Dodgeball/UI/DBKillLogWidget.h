// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBKillLogWidget.generated.h"

class UDBKillLogElementWidget;
class ADBCharacter;
/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBKillLogWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBKillLogWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	virtual void ShowUI() override;

private:
	UPROPERTY()
	TSubclassOf<UDBKillLogElementWidget> KillLogElementClass;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_KillLog;

public:
	void AddKillLogElement(ADBCharacter* InAttacker, ADBCharacter* InVictim);
};
