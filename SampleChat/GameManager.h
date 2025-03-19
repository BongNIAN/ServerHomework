// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameManager.generated.h"

USTRUCT(BlueprintType)
struct SAMPLECHAT_API FGameResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Result")
	int32 Strike;

	UPROPERTY(BlueprintReadOnly, Category = "Result")
	int32 Ball;
};


UCLASS()
class SAMPLECHAT_API UGameManager : public UObject
{
	GENERATED_BODY()
	
public:
	UGameManager();
	
	UFUNCTION(BlueprintCallable, Category = "Logic")
	void InitializeGame();
	
	UFUNCTION(BlueprintCallable, Category = "Logic")
	bool IsValidGuess(const FString& Num) const;

	UFUNCTION(BlueprintCallable, Category = "Logic")
	FGameResult GameResult(const FString& Num);

	UFUNCTION(BlueprintCallable, Category = "Logic")
	FString GetAnswer() const;

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString Answer;
	

	FString GenerateRandomValue() const;


};
