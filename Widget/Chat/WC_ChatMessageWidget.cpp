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
		// �ؽ�Ʈ �ڵ� �ٹٲ�
		TEXT_ChatMessage->SetAutoWrapText(true);
		// �ؽ�Ʈ�� �� �ڽ��� �ʺ�
		TEXT_ChatMessage->SetWrapTextAt(350.f);
		// �ܾ� �ν��ؼ� �ٹٲ�
		TEXT_ChatMessage->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
	}
}

void UWC_ChatMessageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (BeginSound)// �κ��丮 �Ҹ�
	{
		UGameplayStatics::PlaySoundAtLocation(this, BeginSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}
