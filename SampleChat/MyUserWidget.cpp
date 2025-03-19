#include "MyUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "MyPlayerController.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (SubmitButton)
	{
		SubmitButton->OnClicked.AddDynamic(this, &UMyUserWidget::OnSubmitButtonClicked);
	}
}

void UMyUserWidget::UpdateScoreDisplay(const FString& ScoreText)
{
	if (ScoreTextBlock)
	{
		ScoreTextBlock->SetText(FText::FromString(ScoreText));
	}
}

void UMyUserWidget::OnSubmitButtonClicked()
{
	if (!InputTextBox)
	{
		return;
	}

	FString InputString = InputTextBox->GetText().ToString();

	// 소유한 PlayerController를 통해 메시지 전송
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AMyPlayerController* MyPC = Cast<AMyPlayerController>(PC))
		{
			MyPC->SendMessageToServer(InputString);
	
		}
	}

}
