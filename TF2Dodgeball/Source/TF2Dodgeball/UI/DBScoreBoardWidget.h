// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBScoreBoardWidget.generated.h"

class UDBScoreBoardElementWidget;

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBScoreBoardWidget : public UDBUserWidget
{
	GENERATED_BODY()

public:
	UDBScoreBoardWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	virtual void ShowUI() override;
	virtual void HideUI() override;

private:
	UPROPERTY()
	TSubclassOf<UDBScoreBoardElementWidget> ScoreBoardElementClass;

	UPROPERTY()
	TArray<TObjectPtr<UDBScoreBoardElementWidget>> ScoreBoardElementWidgets;

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_ScoreBlue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_ScoreRed;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Spectators;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Kill;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Death;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image_Pyro_Blue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image_Pyro_Red;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_Blue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_Red;
};
