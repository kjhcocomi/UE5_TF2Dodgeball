// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBChatTmpWidget.generated.h"

class ADBCharacter;
/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBChatTmpWidget : public UDBUserWidget
{
	GENERATED_BODY()

public:
	UDBChatTmpWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	virtual void ShowUI() override;

public:
	void AddChat(ADBCharacter* Sender, const FText& ChatText);

private:
	UPROPERTY()
	TSubclassOf<class UDBChatElementWidget> ChatElementClass;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UVerticalBox> VerticalBox_ChatTmp;
};
