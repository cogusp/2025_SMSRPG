void AC_WorldPlayerController::ChatButtonPressed(const FInputActionValue& Value)
{
	AC_WorldHUD* WorldHUD = CastWHUD(GetMyHUD());

	if (WorldHUD)
	{
		WorldHUD->SetChatWidgetVisible(true);
		WorldHUD->ActivateChat();  // 입력 가능 상태로 전환
	}
}

void AC_WorldPlayerController::ServerSendChat_Implementation(const FString& Message)
{
	AC_WorldGameMode* WorldGM = CastWGM(GetMyGM());

	if (WorldGM)
	{
		WorldGM->SendChatMessage(Message);  // 모든 클라이언트에게 브로드캐스트
	}
}

void AC_WorldPlayerController::ClientSendChat_Implementation(const FString& Message)
{
	AC_WorldHUD* WorldHUD = CastWHUD(GetMyHUD());

	if (WorldHUD)
	{
		WorldHUD->AddChatMessage(Message);  // UI에 메시지 출력
	}
}

void AC_WorldGameMode::SendChatMessage(const FString& Message)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AC_WorldPlayerController* WorldPC = Cast<AC_WorldPlayerController>(*It);

		if (WorldPC)
		{
			WorldPC->ClientSendChat(Message);
		}
	}
}
