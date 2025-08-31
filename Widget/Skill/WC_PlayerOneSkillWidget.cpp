#include "Widget/Skill/WC_PlayerOneSkillWidget.h"
#include "Widget/Ingame/WC_SkillSlot.h"
#include "Widget/Skill/WC_SkillInfoWidget.h"

#include "Character/Skill/C_SMSSkill.h"
#include "Components/TextBlock.h"

void UWC_PlayerOneSkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!ToolTipClass) return;
}

void UWC_PlayerOneSkillWidget::SetWidget(FSkillData InSkillData)
{
	SkillSlot->SetSlot(InSkillData);
	SkillSlot->SkillInput = ESkillInput::NONE;
	TEXT_Name->SetText(FText::FromName(InSkillData.SkillData->SkillName));

	// ÅøÆÁ À§Á¬ »ý¼º
	SkillInfoWidget = CreateWidget<UWC_SkillInfoWidget>(this, ToolTipClass);

	if (!SkillInfoWidget) return;

	SkillInfoWidget->SetWidget(InSkillData);

	SetToolTip(SkillInfoWidget);
}