// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBKillLogElementWidget.generated.h"

class ADBCharacter;
/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBKillLogElementWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBKillLogElementWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Attacker;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Victim;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image_Background;

public:
	void SetElement(ADBCharacter* InAttacker, ADBCharacter* InVictim);

private:
	void Remove();

private:
	FTimerHandle RemoveHandle;
};
