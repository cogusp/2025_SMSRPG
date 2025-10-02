void UWC_ChatWidget::OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    // 메시지 공백을 없애고, "닉네임: 채팅" 형태로 가공
	if (CommitMethod == ETextCommit::OnEnter && ChatText)
	{
		FString TrimmedText = ChatText->GetText().ToString().TrimStartAndEnd();

		if (!TrimmedText.IsEmpty())
		{
			AC_WorldPlayerController* WorldPC = CastWPC(GetMyPC());
			APlayerController* PC = GetOwningPlayer();
			AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn());

			if (Player && WorldPC)
			{
				FString Message = FString::Printf(TEXT("%s: %s"), *Player->CharacterInfo.CharacterName, *TrimmedText);
				WorldPC->ServerSendChat(Message);  // 서버에 메시지 전달
			}
		}

		// UI 상태 초기화
		ChatText->SetText(FText::GetEmpty());
		ChatText->SetIsEnabled(false);
		SetChatWidgetVisibility(false);
	}
}
