// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


class UMyUserWidget;

UCLASS()
class SAMPLECHAT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable, Category = "Chat")
	void SendMessageToServer(const FString& Message);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_SendMessage(const FString& Message);

	void Server_SendMessage_Implementation(const FString& Message);

	bool Server_SendMessage_Validate(const FString& Message);


	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateScoreDisplayToClient(const FString& ScoreText);

	UFUNCTION(Client,Reliable)
	void Client_UpdateScoreDisplay(const FString& ScoreText);
	
	void Client_UpdateScoreDisplay_Implementation(const FString& ScoreText);


	UFUNCTION(BlueprintCallable, Category = "Chat")
	void StartInputTimer();

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void CancelInputTimer();


	UFUNCTION(Client, Reliable)
	void Client_StartInputTimer();

	UFUNCTION(Client, Reliable)
	void Client_CancelInputTimer();

	void Client_StartInputTimer_Implementation();
	void Client_CancelInputTimer_Implementation();

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void HandleInputTimeOut();

	UFUNCTION(Server, Reliable)
	void Server_HandleInputTimeout();
	void Server_HandleInputTimeout_Implementation();
	

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void OpenWidget();

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void CloseWidget();


	UFUNCTION(Client, Reliable)
	void Client_OpenWidget();

	UFUNCTION(Client, Reliable)
	void Client_CloseWidget();

	void Client_OpenWidget_Implementation();
	void Client_CloseWidget_Implementation();

	UFUNCTION()
	void OnPlayerUIUpdated(FString Text);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	TSubclassOf<UMyUserWidget> UserWidgetClass;

	TObjectPtr<UMyUserWidget> UserWidget;

	FTimerHandle InputTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timer")
	float TimeoutDuration = 30.f;
};
