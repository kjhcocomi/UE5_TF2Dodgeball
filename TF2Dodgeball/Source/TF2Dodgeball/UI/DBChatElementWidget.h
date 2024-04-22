// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBChatElementWidget.generated.h"

class ADBCharacter;
/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBChatElementWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBChatElementWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	void SetElement(ADBCharacter* Sender, const FText& ChatText, bool IsTmp = false);

private:
	FTimerHandle RemoveHandle;
	void Remove();

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class URichTextBlock> RichTextBlock_Chat;
};
