// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameMode.h"
#include "GameFramework/GameStateBase.h"
#include "GameManager.h"
#include "MyPlayerController.h"

AMyGameMode::AMyGameMode()
{
	CurrentPlayerCount = 0;
	CurrentTurnIndex = 0;
	
}
void AMyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (!HasAuthority())
	{
		return;
	}
	//
	ControllerArray.Add(NewPlayer);
	CurrentPlayerCount++;
	PlayerAttempts.Add(3);
	PlayerScores.Add(0);
	UpdatePlayerCount();
	UE_LOG(LogTemp, Log, TEXT("Player logged in. CurrentPlayerCount: %d"), CurrentPlayerCount);
}
//추가해야할게 로그인하면서 PlayerController와 UserName을 MaP으로 하나 만들고
// 배열을 하나 관리해서 컨트롤러랑 묶을까?
//그걸 관리하고 있다면 좀 수월할것 같은데?
void AMyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (!HasAuthority())
	{
		return;
	}
	if (APlayerController* ExitingPC = Cast<APlayerController>(Exiting))
	{
		int32 Index = ControllerArray.IndexOfByKey(ExitingPC);
		if (Index <= PlayerIndex)
		{
			
		}
		ControllerArray.Remove(ExitingPC);

	}

	CurrentPlayerCount--;
	UpdatePlayerCount();
	UE_LOG(LogTemp, Log, TEXT("Player logged out. CurrentPlayerCount: %d"), CurrentPlayerCount);
}
void AMyGameMode::UpdatePlayerCount()
{
	//if (GetWorld() && GetWorld()->GetGameState<AGameStateBase>())
	//{
	//	CurrentPlayerCount = GetWorld()->GetGameState<AGameStateBase>()->PlayerArray.Num();
	//}
}
void AMyGameMode::ProcessInput(const FString& Input)
{
	if (!HasAuthority())
	{
		return;
	}
	if (!Input.StartsWith(TEXT("/")))
	{
		SwitchTurn();
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("InGameMode Input Success"));
	FString UserGuess = Input.RightChop(1);
	//ensure(GameManager);
	if (!GameManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Game Manager"));
	}
	if (!GameManager->IsValidGuess(UserGuess))
	{
		UE_LOG(LogTemp, Warning, TEXT("올바르지 않은 입력이오 기회가 차감되오"));
		PlayerAttempts[CurrentTurnIndex]--;
		SwitchTurn();
		return;
	}
	FGameResult GameResult = GameManager->GameResult(UserGuess);
	int32 Strikes = GameResult.Strike;
	int32 Balls = GameResult.Ball;
	
	FString StrikeBallString = FString::Printf(TEXT("Player%d Input : %s,%d S %d B"),
		CurrentTurnIndex, *UserGuess, Strikes, Balls);
	OnPlayerUIUpdated.Broadcast(StrikeBallString);

	if (Strikes == 3)
	{
		UE_LOG(LogTemp, Log, TEXT("%s Won! Game will reset."), *GetPlayerName());
		PlayerScores[CurrentTurnIndex]++;
		FString FormattedString = FString::Printf(TEXT("Player%d Win,  has scored %d points!"), PlayerIndex, PlayerScores[PlayerIndex]);
		OnPlayerUIUpdated.Broadcast(FormattedString);
		ResetGame();
		return;
	}

	PlayerAttempts[CurrentTurnIndex]--;
	SwitchTurn();
}

void AMyGameMode::UpdateOnControllers()
{
	FString ScoreText;
	for (int32 i = 0; i < NumPlayers; ++i)
	{
		ScoreText += FString::Printf(TEXT("Player%d Score: %d  "), i + 1, PlayerScores[i]);
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
	
	}
}

int32 AMyGameMode::GetRandomIndex()
{
	return FMath::RandRange(0, CurrentPlayerCount - 1);
}

void AMyGameMode::ResetGame()
{
	if (GameManager)
	{
		GameManager->InitializeGame();
	}

	CurrentTurnIndex = GetRandomIndex();

	for (int32 i = 0; i < CurrentPlayerCount; i++)
	{
		PlayerAttempts[i] = 3;
	}

	if (AMyPlayerController* StartPlayer = Cast<AMyPlayerController>(ControllerArray[CurrentTurnIndex]))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameModeBeginPlay In Func"));
		StartPlayer->OpenWidget();
		StartPlayer->StartInputTimer();
	}

	OnPlayerUIUpdated.Broadcast("RestartGame");
	
	//Todo : UpdateUI

	
	////if (GetWorld() && GetWorld()->GetGameState<AGameStateBase>())
	//{
	//	CurrentPlayerCount = GetWorld()->GetGameState<AGameStateBase>()->PlayerArray.Num();
	//}
}

FString AMyGameMode::GetPlayerName() const
{
	
	return FString();
}

void AMyGameMode::SwitchTurn()
{
	/**Close Widget && Cancel Timer */
	if (!ControllerArray[CurrentTurnIndex].IsNull())
	{
		TObjectPtr<AMyPlayerController> TmpController = Cast<AMyPlayerController>(ControllerArray[CurrentTurnIndex]);
	
		TmpController->CloseWidget();
		TmpController->CancelInputTimer();

	}

	
	if ((CurrentTurnIndex + 1) == CurrentPlayerCount)
	{
		CurrentTurnIndex = (CurrentTurnIndex + 1) % CurrentPlayerCount;
	}
	else
	{
		CurrentTurnIndex++;
	}

	int32 TestDebug = 0;
	while (1)
	{
		/**지금 이부분만 디버깅하면 턴제는 적용되는건데 */
		if (TestDebug == CurrentPlayerCount)
		{
			//
			for (int32 i = 0; i < CurrentPlayerCount; i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerAttempts[i]);
			}
			UE_LOG(LogTemp, Warning, TEXT("Nobody has Attempts"));
			OnPlayerUIUpdated.Broadcast("Nobody has Attempts , Restart Game");
			ResetGame();
			return;
		}
		
		if (PlayerAttempts[CurrentTurnIndex] == 0)
		{
			TestDebug++;
			
			if ((CurrentTurnIndex + 1) == CurrentPlayerCount)
			{
				CurrentTurnIndex = (CurrentTurnIndex + 1) % CurrentPlayerCount;
			}
			else
			{
				CurrentTurnIndex++;
			}
			continue;
		}
		else
		{
			break;
		}

	}
	UE_LOG(LogTemp, Warning, TEXT("루프 탈출 "));
	/**Open Widget && Start Timer */
	
	if (!ControllerArray[CurrentTurnIndex].IsNull())
	{
		TObjectPtr<AMyPlayerController> TmpController = Cast<AMyPlayerController>(ControllerArray[CurrentTurnIndex]);

		TmpController->OpenWidget();
		TmpController->StartInputTimer();
	}
	

}

void AMyGameMode::ReducePlayerChance(APlayerController* Player)
{
	if (!HasAuthority() || !Player)
	{
		return;
	}
	PlayerAttempts[CurrentTurnIndex]--;
	UE_LOG(LogTemp, Warning, TEXT("TimeOver"));
	SwitchTurn();
}

void AMyGameMode::BeginPlay()
{
	

	//ensure(GameManager);
	if (GameManagerClass)
	{
		GameManager = NewObject<UGameManager>(this, GameManagerClass);

	}
	FTimerHandle InitTimer;
	GetWorldTimerManager().SetTimer(InitTimer, this, &AMyGameMode::ResetGame, 5.0f, false);

	
}
