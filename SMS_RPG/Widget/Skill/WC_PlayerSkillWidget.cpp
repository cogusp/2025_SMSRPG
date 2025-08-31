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

	// 창 닫기 버튼 초기화
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// 창 닫기 버튼에 함수 바인딩
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerSkillWidget::OnCloseButtonClicked);

	// 스킬 리스트 생성
	SetSkillList();

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (ButtonSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}

void UWC_PlayerSkillWidget::SetSkillList()
{
	if (!SkillSlotClass || !SkillScrollBox || !SkillComponent) return;

	// 스크롤 박스 내용물 지우기
	SkillScrollBox->ClearChildren();

	TArray<FSkillData> SkillList = SkillComponent->GetOwnedSkills();

	for (int32 i = 0; i < SkillList.Num(); ++i)
	{
		// 스킬 위젯 생성
		UWC_PlayerOneSkillWidget* Skill = CreateWidget<UWC_PlayerOneSkillWidget>(this, SkillSlotClass);

		// 스킬 정보 설정
		Skill->SetWidget(SkillList[i]);
		// 여백 설정
		Skill->SetPadding(FMargin(4));

		// 스크롤 박스에 추가
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

	if (ButtonSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}