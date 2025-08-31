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

	// Text commit 콜백 함수 바인딩
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
		// 채팅창 활성화
		SetVisibility(ESlateVisibility::HitTestInvisible);
		// 채팅창 배경 활성화
		BorderChatText->SetVisibility(ESlateVisibility::Visible);
		// 채팅 입력 텍스트 활성화
		Background->SetVisibility(ESlateVisibility::Visible);
		// 스크롤바 활성화
		ChatScrollBox->SetScrollBarVisibility(ESlateVisibility::Visible);
	}
	else
	{
		// 채팅창 배경 비활성화
		BorderChatText->SetVisibility(ESlateVisibility::Collapsed);
		// 채팅 입력 텍스트 비활성화
		Background->SetVisibility(ESlateVisibility::Collapsed);
		// 스크롤바 활성화
		ChatScrollBox->SetScrollBarVisibility(ESlateVisibility::Collapsed);
	}

	// 스크롤 활성화/비활성화
	ChatScrollBox->SetIsEnabled(bOnShow);
}

void UWC_ChatWidget::AddChatMessage(UWC_ChatMessageWidget* Message)
{
	ChatScrollBox->AddChild(Message);
	ChatScrollBox->ScrollToEnd();
	ChatScrollBox->bAnimateWheelScrolling = true;
}

// Enter 키 입력 시 채팅 전송
void UWC_ChatWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// Enter 키 입력 확인
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (ChatText)
		{
			// 입력된 텍스트의 좌우 공백 제거
			FText InputText = ChatText->GetText();
			FString TrimmedText = InputText.ToString().TrimStartAndEnd();

			// 현재 월드의 첫 번째 플레이어 컨트롤러 가져옴
			AC_WorldPlayerController* WorldPC = CastWPC((GetMyPC()));

			if (WorldPC)
			{
				// 공백이 아닌 텍스트가 존재하는 경우만 실행
				if (!TrimmedText.IsEmpty())
				{
					// 플레이어의 이름을 가져오기 위해 연결된 플레이어 가져옴
					APlayerController* PC = GetOwningPlayer();

					if (PC)
					{
						AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn());

						if (Player)
						{
							// 캐릭터 정보 가져옴
							FCharacterInfo PlayerInfo = Player->CharacterInfo;

							// TrimmedText 앞에 UserName을 붙여 최종 Message 생성
							FString Message = FString::Printf(TEXT("%s: %s"), *PlayerInfo.CharacterName, *TrimmedText);

							// 채팅 메시지를 보내지 위한 Server RPC 호출
							WorldPC->ServerSendChat(Message);
						}
					}
				}

				// 입력 모드를 게임 전용으로 변경하여 채팅 입력 비활성화
				AC_WorldHUD* WorldHUD = CastWHUD(GetMyHUD());
				WorldHUD->SetInputMode(EInputMode::GameOnly, false);

				// 채팅 입력 필드를 비우고 비활성화
				ChatText->SetText(FText::GetEmpty());
				ChatText->SetIsEnabled(false);

				// 채팅창 비활성화
				SetChatWidgetVisibility(false);
			}
		}
	}
}
