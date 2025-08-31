#include "Widget/Chat/WC_ChatMessageWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"

void UWC_ChatMessageWidget::SetChatMessage(const FString& Message)
{
	if (TEXT_ChatMessage)
	{
		TEXT_ChatMessage->SetText(FText::FromString(Message));
		TEXT_ChatMessage->Font.Size = 14;
		// 텍스트 자동 줄바꿈
		TEXT_ChatMessage->SetAutoWrapText(true);
		// 텍스트가 들어갈 박스의 너비
		TEXT_ChatMessage->SetWrapTextAt(350.f);
		// 단어 인식해서 줄바꿈
		TEXT_ChatMessage->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
	}
}

void UWC_ChatMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (BeginSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, BeginSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}
