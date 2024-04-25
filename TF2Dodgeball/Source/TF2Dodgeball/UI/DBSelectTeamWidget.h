// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBSelectTeamWidget.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBSelectTeamWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBSelectTeamWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Spectator;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Red;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Blue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Quit;

private:
	UFUNCTION()
	void PushSpectatorButton();

	UFUNCTION()
	void PushRedButton();

	UFUNCTION()
	void PushBlueButton();

	UFUNCTION()
	void PushQuitButton();
};
