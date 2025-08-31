#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_ChatMessageWidget.generated.h"

UCLASS()
class STRONGMETALSTONE_API UWC_ChatMessageWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
public:
	// ���۵� �޽����� �����ϴ� �Լ�
	void SetChatMessage(const FString& Message);

	virtual void NativeConstruct() override;

private:
	// ���۵� �޽���
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_ChatMessage;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BeginSound;
};
