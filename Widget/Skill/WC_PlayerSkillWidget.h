#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_PlayerSkillWidget.generated.h"

class UScrollBox;
class UWC_PlayerOneSkillWidget;
class UC_SkillSystemComponent;

UCLASS()
class STRONGMETALSTONE_API UWC_PlayerSkillWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct();

public:
	void SetSkillList();

	// 나가기 버튼 누르면 실행
	UFUNCTION()
	void OnCloseButtonClicked();
	
public:
	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;	// 나가기 버튼

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SkillScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillRow")
	TSubclassOf<UWC_PlayerOneSkillWidget> SkillSlotClass;

	UC_SkillSystemComponent* SkillComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ButtonSound;
};
