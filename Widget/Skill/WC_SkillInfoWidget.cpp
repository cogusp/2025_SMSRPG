#include "Widget/Skill/WC_SkillInfoWidget.h" 
#include "Character/Skill/C_SMSSkill.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWC_SkillInfoWidget::SetWidget(FSkillData InSkillData)
{
	if (!IMG_Skill || !TEXT_SkillName || !TEXT_Level || !TEXT_SkillType || !TEXT_Mana || !TEXT_Cooldown || !TEXT_Description) return;

	if (!InSkillData.SkillData) return;

	IMG_Skill->SetBrushFromTexture(InSkillData.SkillData->Icon);
	TEXT_SkillName->SetText(FText::FromName(InSkillData.SkillData->SkillName));
	TEXT_Level->SetText(FText::AsNumber(InSkillData.SkillData->UnlockedLevel));

	if (InSkillData.SkillData->SkillType == ESkillType::ONCE)
	{
		TEXT_SkillType->SetText(FText::FromString(TEXT("단발성")));
	}
	else if (InSkillData.SkillData->SkillType == ESkillType::HOLD)
	{
		TEXT_SkillType->SetText(FText::FromString(TEXT("홀드")));
	}
	
	TEXT_Mana->SetText(FText::AsNumber(InSkillData.SkillData->DemandedMana));
	TEXT_Cooldown->SetText(FText::AsNumber(InSkillData.SkillData->CoolTime));
	TEXT_Description->SetText(InSkillData.SkillData->Description);
}
