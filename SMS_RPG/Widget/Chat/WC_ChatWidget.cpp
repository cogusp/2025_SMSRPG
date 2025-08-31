#include "Widget/Chat/WC_ChatWidget.h"
#include "Widget/Chat/WC_ChatMessageWidget.h"

#include "HUD/C_WorldHUD.h"
#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"

#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "Components/Image.h"
#include "Components/Border.h"

void UWC_ChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Text commit �ݹ� �Լ� ���ε�
	if (ChatText)
	{
		ChatText->OnTextCommitted.AddDynamic(this, &UWC_ChatWidget::OnTextCommitted);
	}
}

void UWC_ChatWidget::ActivateChat()
{
	if (ChatText)
	{
		ChatText->SetIsEnabled(true);
		ChatText->SetFocus();
	}
}

void UWC_ChatWidget::SetChatWidgetVisibility(bool bOnShow)
{
	if (bOnShow)
	{
		// ä��â Ȱ��ȭ
		SetVisibility(ESlateVisibility::HitTestInvisible);
		// ä��â ��� Ȱ��ȭ
		BorderChatText->SetVisibility(ESlateVisibility::Visible);
		// ä�� �Է� �ؽ�Ʈ Ȱ��ȭ
		Background->SetVisibility(ESlateVisibility::Visible);
		// ��ũ�ѹ� Ȱ��ȭ
		ChatScrollBox->SetScrollBarVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// ä��â ��� ��Ȱ��ȭ
		BorderChatText->SetVisibility(ESlateVisibility::Collapsed);
		// ä�� �Է� �ؽ�Ʈ ��Ȱ��ȭ
		Background->SetVisibility(ESlateVisibility::Collapsed);
		// ��ũ�ѹ� Ȱ��ȭ
		ChatScrollBox->SetScrollBarVisibility(ESlateVisibility::Collapsed);
	}

	// ��ũ�� Ȱ��ȭ/��Ȱ��ȭ
	ChatScrollBox->SetIsEnabled(bOnShow);
}

void UWC_ChatWidget::AddChatMessage(UWC_ChatMessageWidget* Message)
{
	ChatScrollBox->AddChild(Message);
	ChatScrollBox->ScrollToEnd();
	ChatScrollBox->bAnimateWheelScrolling = true;
}

// Enter Ű �Է� �� ä�� ����
void UWC_ChatWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// Enter Ű �Է� Ȯ��
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (ChatText)
		{
			// �Էµ� �ؽ�Ʈ�� �¿� ���� ����
			FText InputText = ChatText->GetText();
			FString TrimmedText = InputText.ToString().TrimStartAndEnd();

			// ���� ������ ù ��° �÷��̾� ��Ʈ�ѷ� ������
			AC_WorldPlayerController* WorldPC = CastWPC((GetMyPC()));

			if (WorldPC)
			{
				// ������ �ƴ� �ؽ�Ʈ�� �����ϴ� ��츸 ����
				if (!TrimmedText.IsEmpty())
				{
					// �÷��̾��� �̸��� �������� ���� ����� �÷��̾� ������
					APlayerController* PC = GetOwningPlayer();

					if (PC)
					{
						AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn());

						if (Player)
						{
							// ĳ���� ���� ������
							FCharacterInfo PlayerInfo = Player->CharacterInfo;

							// TrimmedText �տ� UserName�� �ٿ� ���� Message ����
							FString Message = FString::Printf(TEXT("%s: %s"), *PlayerInfo.CharacterName, *TrimmedText);

							// ä�� �޽����� ������ ���� Server RPC ȣ��
							WorldPC->ServerSendChat(Message);
						}
					}
				}

				// �Է� ��带 ���� �������� �����Ͽ� ä�� �Է� ��Ȱ��ȭ
				AC_WorldHUD* WorldHUD = CastWHUD(GetMyHUD());
				WorldHUD->SetInputMode(EInputMode::GameOnly, false);

				// ä�� �Է� �ʵ带 ���� ��Ȱ��ȭ
				ChatText->SetText(FText::GetEmpty());
				ChatText->SetIsEnabled(false);

				// ä��â ��Ȱ��ȭ
				SetChatWidgetVisibility(false);
			}
		}
	}
}
