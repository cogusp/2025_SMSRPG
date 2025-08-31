#pragma once

#include "Widget/WC_SMSUserWidget.h"
#include "CoreMinimal.h"
#include "WC_ChatWidget.generated.h"

UCLASS()
class STRONGMETALSTONE_API UWC_ChatWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Enter 키 입력 시 채팅창을 활성화하기 위한 함수
	UFUNCTION()
	void ActivateChat();

	// 채팅창을 보이게 세팅하는 함수
	UFUNCTION()
	void SetChatWidgetVisibility(bool bOnShow);

	// 채팅창에 메시지를 추가하는 함수
	UFUNCTION()
	void AddChatMessage(UWC_ChatMessageWidget* Message);

protected:
	// Enter 키 입력 시 채팅 전송
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	// 채팅 입력을 위한 텍스트
	UPROPERTY(meta = (BindWidget))
	class UEditableText* ChatText;

	// 채팅 기록을 위한 스크롤 박스
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatScrollBox;

	// 스크롤 박스의 배경
	UPROPERTY(meta = (BindeWidget))
	class UImage* Background;

	// 채팅 입력 텍스트의 배경
	UPROPERTY(meta = (BindWidget))
	class UBorder* BorderChatText;
};
