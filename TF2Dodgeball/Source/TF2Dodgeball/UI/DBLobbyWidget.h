// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBLobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBLobbyWidget : public UDBUserWidget
{
	GENERATED_BODY()

public:
	UDBLobbyWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> TextBox_IP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UEditableTextBox> TextBox_Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Start;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UButton> Button_Quit;

private:
	UFUNCTION()
	void PushStartButton();

	UFUNCTION()
	void PushQuitButton();
};
