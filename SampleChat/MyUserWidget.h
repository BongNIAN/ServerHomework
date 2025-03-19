// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

UCLASS()
class SAMPLECHAT_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;

	
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateScoreDisplay(const FString& ScoreText);

protected:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> InputTextBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SubmitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoreTextBlock;

	
	UFUNCTION()
	void OnSubmitButtonClicked();
};
