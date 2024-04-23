// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DBUserWidget.h"
#include "DBHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class TF2DODGEBALL_API UDBHudWidget : public UDBUserWidget
{
	GENERATED_BODY()
	
public:
	UDBHudWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeConstruct() override;

public:
	virtual void ShowUI() override;

private:
	void SetPercent();

public:
	void SetImage(class ADBCharacter* Player);
	void StartCoolDown(float InCoolTime);

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image_Pyro_Blue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image_Pyro_Red;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UImage> Image_Dead;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<class UProgressBar> ProgressBar_CoolDown;

private:
	float CurrentSecond = 1.f;
	float CoolTime = 1.f;
	float TickSecond = 0.05f;
	FTimerHandle CoolDownHandle;
};
