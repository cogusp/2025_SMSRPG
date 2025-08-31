#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_SkillInfoWidget.generated.h"

class UImage;
class UTextBlock;

UCLASS()
class STRONGMETALSTONE_API UWC_SkillInfoWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	void SetWidget(FSkillData InSkillData);

public:

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Skill;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_SkillName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Level;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_SkillType;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Mana;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Cooldown;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Description;
};
