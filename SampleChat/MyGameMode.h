// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

class UGameManager;
class APlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerUIUpdated, FString, Text);
/**
 * 
 */
UCLASS()
class SAMPLECHAT_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AMyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void ProcessInput(const FString& Input);

	void UpdateOnControllers();

	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void UpdatePlayerCount();
	
	UFUNCTION(BlueprintCallable, Category = "GameLogic")
	void ReducePlayerChance(APlayerController* APlayer);
	
	//관리자용 브로드캐스트 하나 놓고, text하나 뿌려줘야겠네 
	// Text안에 1s 2b이런거 담아주고 
	UPROPERTY(BlueprintAssignable, Category = "GameLogic")
	FOnPlayerUIUpdated OnPlayerUIUpdated;

	int32 GetRandomIndex();

protected:
	void ResetGame();
	FString GetPlayerName() const;
	void SwitchTurn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameLogic")
	TSubclassOf<UGameManager> GameManagerClass;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "GameLogic")
	TObjectPtr<UGameManager> GameManager;

	

	TArray<int32> PlayerAttempts;
	int32 CurrentTurnIndex;
	TArray<int32> PlayerScores;
	int32 PlayerIndex=0;
	
	void BeginPlay() override;
private:

	int32 CurrentPlayerCount;
	TMap<int32,APlayerController*> ControllerByIndex;
	TArray<TObjectPtr<APlayerController>> ControllerArray;
}; 
