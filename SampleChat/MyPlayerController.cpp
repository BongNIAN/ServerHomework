// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyUserWidget.h"
#include "MyGameMode.h"
#include "Engine/Engine.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocalController())
	{
		if (UserWidgetClass)
		{
			UserWidget = CreateWidget<UMyUserWidget>(this, UserWidgetClass);
			if (UserWidget)
			{
				UserWidget->AddToViewport();
				UserWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	
	if (IsLocalController() && GetWorld() && GetWorld()->GetAuthGameMode())
	{
		AMyGameMode* GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnPlayerUIUpdated.AddDynamic(this, &AMyPlayerController::OnPlayerUIUpdated);
		}
	}
	
}

void AMyPlayerController::SendMessageToServer(const FString& Message)
{
	if (!Message.IsEmpty())
	{
		Server_SendMessage(Message);
	}
}

void AMyPlayerController::Server_SendMessage_Implementation(const FString& Message)
{
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("No authority."));
		return;
	}

	if (GetWorld())
	{
		if (AMyGameMode* GameMode= Cast<AMyGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GameMode->ProcessInput(Message);
		}
	}
}

bool AMyPlayerController::Server_SendMessage_Validate(const FString& Message)
{
	//TestSample
	return true;
}

void AMyPlayerController::UpdateScoreDisplayToClient(const FString& ScoreText)
{
	Client_UpdateScoreDisplay(ScoreText);
}

void AMyPlayerController::Client_UpdateScoreDisplay_Implementation(const FString& ScoreText)
{
	if (!IsLocalController())
	{
		UE_LOG(LogTemp, Display, TEXT("No Local Controller"));
		return;

	}
	if (UserWidget)
	{
		UserWidget->UpdateScoreDisplay(ScoreText);
	}
}

void AMyPlayerController::StartInputTimer()
{
	Client_StartInputTimer();
}

void AMyPlayerController::CancelInputTimer()
{
	Client_CancelInputTimer();
}

void AMyPlayerController::Client_StartInputTimer_Implementation()
{
	GetWorldTimerManager().SetTimer(InputTimerHandle, 
		this,
		&AMyPlayerController::Server_HandleInputTimeout,
		TimeoutDuration,
		false);
}

void AMyPlayerController::Client_CancelInputTimer_Implementation()
{
	if (GetWorldTimerManager().IsTimerActive(InputTimerHandle)) 
	{
		GetWorldTimerManager().ClearTimer(InputTimerHandle);
	}
	
}

void AMyPlayerController::HandleInputTimeOut()
{
	Server_HandleInputTimeout();
}

void AMyPlayerController::Server_HandleInputTimeout_Implementation()
{
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		AMyGameMode* GameMode = Cast<AMyGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			// 서버 RPC로 기회 감소 로직 실행
			GameMode->ReducePlayerChance(this);
		}
	}
}

void AMyPlayerController::OpenWidget()
{
	Client_OpenWidget();
	
}

void AMyPlayerController::CloseWidget()
{
	Client_CloseWidget();
}

void AMyPlayerController::Client_OpenWidget_Implementation()
{
	//IsLocalController() && 
	if (UserWidget)
	{
		UserWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AMyPlayerController::Client_CloseWidget_Implementation()
{
	//IsLocalController()&&
	if (  UserWidget)
	{
		UserWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyPlayerController::OnPlayerUIUpdated(FString Text)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, Text);
	}
}