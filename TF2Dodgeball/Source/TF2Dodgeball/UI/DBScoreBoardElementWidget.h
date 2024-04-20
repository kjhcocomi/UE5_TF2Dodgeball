// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/DBEnums.h"
#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBScoreBoardElementWidget.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBScoreBoardElementWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBScoreBoardElementWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image_Background;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Name;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Kill;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UTextBlock> Text_Death;

public:
	void SetElement(FString InName, int InKill, int InDeath, TeamColor InTeamColor, bool IsMyCharacter, bool IsDead);
};
