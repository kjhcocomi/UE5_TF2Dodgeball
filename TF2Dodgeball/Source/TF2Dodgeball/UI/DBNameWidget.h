// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBNameWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBNameWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void NativeConstruct() override;

public:
	void SetNameText(FText InName);

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<class UTextBlock> Text_Name;
};
