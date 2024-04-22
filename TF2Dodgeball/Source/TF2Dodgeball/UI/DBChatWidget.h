// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBChatWidget.generated.h"

class ADBCharacter;
/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBChatWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBChatWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	virtual void ShowUI() override;
	virtual void HideUI() override;

public:
	UFUNCTION()
	void DelegateCommitText(const FText& Text, ETextCommit::Type CommitMethod);

public:
	void AddChat(ADBCharacter* Sender, const FText& ChatText);

private:
	UPROPERTY()
	TSubclassOf<class UDBChatElementWidget> ChatElementClass;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UScrollBox> ScrollBox_Chat;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UEditableText> EditableText_Chat;
};
