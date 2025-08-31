#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_ChatMessageWidget.generated.h"

UCLASS()
class STRONGMETALSTONE_API UWC_ChatMessageWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
public:
	// 전송될 메시지를 설정하는 함수
	void SetChatMessage(const FString& Message);

	virtual void NativeConstruct() override;

private:
	// 전송될 메시지
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_ChatMessage;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BeginSound;
};
