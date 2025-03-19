#include "GameManager.h"


UGameManager::UGameManager()
{
    //InitializeGame();
}

void UGameManager::InitializeGame()
{
    Answer = GenerateRandomValue();
}

bool UGameManager::IsValidGuess(const FString& Num) const
{
    if (Num.Len() != 3)
    {
        return false;
    }
    TSet<TCHAR> NumDigits;
    
	for (TCHAR Char : Num)
	{
		if (Char < '1' || Char > '9')
		{
			return false;
		}
		if (NumDigits.Contains(Char))
		{
			return false;
		}
		NumDigits.Add(Char);
	}
	return true;

}

FGameResult UGameManager::GameResult(const FString& Num)
{
	FGameResult GameResult = {0};
	int32 Strike = 0;
	int32 Ball = 0;
	int32 i = 0;
	int32 Index = 0;

	for ( i=0;i<Num.Len();i++)
	{
		/**Test For My Study*/
		TCHAR tmp[2] = { Num[i],'\0' };
		FString TmpString(tmp);

		if (Num[i] == Answer[i])
		{
			GameResult.Strike++;
		}
		else if (Answer.FindChar(Num[i], Index))
		{
			GameResult.Ball++;
		}
		/**Test For My Study*/
		else
		{
			if (Answer.Contains(TmpString))
			{
				UE_LOG(LogTemp, Warning, TEXT("Success Ball"));
			}
		}
	}

	return GameResult;
}

FString UGameManager::GetAnswer() const
{
    return Answer;
}

FString UGameManager::GenerateRandomValue() const
{
	TArray<int32> SelectedDigits;
	while (SelectedDigits.Num() < 3)
	{
		int32 SelectedDigit = FMath::RandRange(1, 9);
		if (!SelectedDigits.Contains(SelectedDigit))
		{
			SelectedDigits.Add(SelectedDigit);
		}
	}
	FString RandomValue = FString::Printf(TEXT("%d%d%d"), SelectedDigits[0], SelectedDigits[1], SelectedDigits[2]);
	UE_LOG(LogTemp, Warning, TEXT("Random is %s"),*RandomValue);
	return  RandomValue;
}
