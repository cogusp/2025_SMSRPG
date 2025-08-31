#include "Widget/Skill/WC_PlayerSkillWidget.h"
#include "Widget/Skill/WC_PlayerOneSkillWidget.h"
#include "Widget/WC_SMSButton.h"

#include "Components/ScrollBox.h"

#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Component/C_SkillSystemComponent.h"
#include "HUD/C_WorldHUD.h"

void UWC_PlayerSkillWidget::NativeConstruct()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn()))
		{
			SkillComponent = Player->GetSkillComponent();
		}
	}

	// â �ݱ� ��ư �ʱ�ȭ
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// â �ݱ� ��ư�� �Լ� ���ε�
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerSkillWidget::OnCloseButtonClicked);

	// ��ų ����Ʈ ����
	SetSkillList();

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (ButtonSound)// �κ��丮 �Ҹ�
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}

void UWC_PlayerSkillWidget::SetSkillList()
{
	if (!SkillSlotClass || !SkillScrollBox || !SkillComponent) return;

	// ��ũ�� �ڽ� ���빰 �����
	SkillScrollBox->ClearChildren();

	TArray<FSkillData> SkillList = SkillComponent->GetOwnedSkills();

	for (int32 i = 0; i < SkillList.Num(); ++i)
	{
		// ��ų ���� ����
		UWC_PlayerOneSkillWidget* Skill = CreateWidget<UWC_PlayerOneSkillWidget>(this, SkillSlotClass);

		// ��ų ���� ����
		Skill->SetWidget(SkillList[i]);
		// ���� ����
		Skill->SetPadding(FMargin(4));

		// ��ũ�� �ڽ��� �߰�
		SkillScrollBox->AddChild(Skill);
	}
}

void UWC_PlayerSkillWidget::OnCloseButtonClicked()
{
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetPlayerSkillWidgetVisible(false);
	}

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (ButtonSound)// �κ��丮 �Ҹ�
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}