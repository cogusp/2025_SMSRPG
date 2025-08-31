#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_PlayerOneSkillWidget.generated.h"

class UWC_SkillSlot;
class UC_SMSSkill;
class UTextBlock;
class UWC_SkillInfoWidget;

UCLASS()
class STRONGMETALSTONE_API UWC_PlayerOneSkillWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:

	UFUNCTION()
	void SetWidget(FSkillData InSkillData);

private:

	// 위젯 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWC_SkillInfoWidget> ToolTipClass;

	// 위젯 인스턴스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWC_SkillInfoWidget> SkillInfoWidget;

public:

	UPROPERTY(meta = (BindWidget))
	UWC_SkillSlot* SkillSlot;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Name;
};
