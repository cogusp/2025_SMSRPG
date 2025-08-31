#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_PopupWidget.generated.h"

class UWC_SMSButton;

UCLASS()
class STRONGMETALSTONE_API UWC_PopupWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetNoticeText(const FText& text);

	// 나가기 버튼 누르면 실행
	UFUNCTION()
	void OnCloseButtonClicked();
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_Log;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_Check;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* CloseButton;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* WarningSound;
};
