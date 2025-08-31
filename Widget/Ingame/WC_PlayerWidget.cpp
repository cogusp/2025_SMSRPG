#include "Widget/Ingame/WC_PlayerWidget.h"
#include "Widget/Ingame/WC_HotbarWidget.h"
#include "Widget/Chat/WC_ChatWidget.h"
#include "Widget/Party/WC_PartyWidget.h"
#include "Widget/WC_SMSProgressBar.h"
#include "Widget/WC_SMSButton.h"
#include "Widget/Ingame/WC_MyInfoWidget.h"

#include "HUD/C_WorldHUD.h"
#include "Character/C_PlayerCharacter.h"
#include "Animation/WidgetAnimation.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"

UWC_PlayerWidget::UWC_PlayerWidget()
{
	// Material 불러오기
}

void UWC_PlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 채팅 버튼 이벤트 바인딩
	ChatWidget->SetChatWidgetVisibility(false);

	// Progress Bar 초기화
	InitProgressBar();

	// 버튼 초기화
	InitButtons();
}

void UWC_PlayerWidget::InitProgressBar()
{
	if (ExpBar)
	{
		ExpBar->SetType(EProgressBarType::EXP);
		ExpBar->Init(0.0f, FLinearColor::Green);
	}

	if (HealthBar)
	{
		HealthBar->SetType(EProgressBarType::HEALTH);
		HealthBar->Init(1.0f, FLinearColor::Red);
	}

	if (ManaBar)
	{
		ManaBar->SetType(EProgressBarType::MANA);
		ManaBar->Init(1.0f, FLinearColor::Blue);
	}

	if (BossBar)
	{
		BossBar->SetType(EProgressBarType::BOSS);
		BossBar->Init(1.0f, FLinearColor::Red);
	}
}

void UWC_PlayerWidget::UpdateProgressBar(EProgressBarType type, float percent)
{
	switch (type)
	{
	case EProgressBarType::EXP:
		if (ExpBar) ExpBar->Update(percent);
		break;
	case EProgressBarType::HEALTH:
		if (HealthBar) HealthBar->Update(percent);
		break;
	case EProgressBarType::MANA:
		if (ManaBar) ManaBar->Update(percent);
		break;
	case EProgressBarType::BOSS:
		if (BossBar) BossBar->Update(percent);
		break;
	}
}

void UWC_PlayerWidget::UpdateAllUI()
{
	if (!CastWCharacter(GetMyCharacter())) return;

	FCharacterInfo CharacterInfo = CastWCharacter(GetMyCharacter())->CharacterInfo;	

	if (ExpBar)	
		ExpBar->Update(CharacterInfo.CurExp / CharacterInfo.MaxExp);
	
	if (HealthBar)
		HealthBar->Update(CharacterInfo.Curhp / CharacterInfo.Maxhp);

	if (ManaBar)
		ManaBar->Update(CharacterInfo.Curmp / CharacterInfo.Maxmp);

	// 파티 UI Update
	if (WBPC_PartyWidget)
		WBPC_PartyWidget->Update();

}

void UWC_PlayerWidget::SetVisibilityUnderUI(bool bOnShow)
{
	if (bOnShow)
	{
		if (ExpBar)
			ExpBar->SetVisibility(ESlateVisibility::Visible);

		if (HealthBar)
			HealthBar->SetVisibility(ESlateVisibility::Visible);

		if (ManaBar)
			ManaBar->SetVisibility(ESlateVisibility::Visible);

		if (HotbarWidget)
			HotbarWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		if (ExpBar)
			ExpBar->SetVisibility(ESlateVisibility::Collapsed);

		if (HealthBar)
			HealthBar->SetVisibility(ESlateVisibility::Collapsed);

		if (ManaBar)
			ManaBar->SetVisibility(ESlateVisibility::Collapsed);

		if (HotbarWidget)
			HotbarWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWC_PlayerWidget::InitButtons()
{
	// 메뉴 버튼 설정
	if (MenuButton)
	{
		// 버튼 초기화
		MenuButton->Init(FText::FromString(TEXT("메뉴")), 10, LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Menu")));

		// 함수 이벤트 바인딩
		MenuButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerWidget::OnMenuButtonClicked);

		SetVisibilityButtons(MenuButton, true);
	}

	// 설정 버튼 설정
	if (SettingButton)
	{
		// 버튼 초기화
		SettingButton->Init(FText::FromString(TEXT("설정")), 10, LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Setting")));

		// 함수 이벤트 바인딩
		SettingButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerWidget::OnSettingButtonClicked);

		SetVisibilityButtons(SettingButton, false);
	}

	// 내 정보 버튼 설정
	if (MyInfoButton)
	{
		// 버튼 초기화
		MyInfoButton->Init(FText::FromString(TEXT("내 정보")), 10, LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/MyInfo")));

		// 함수 이벤트 바인딩
		MyInfoButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerWidget::OnMyInfoButtonClicked);

		SetVisibilityButtons(MyInfoButton, false);
	}

	// 인벤토리 버튼 설정
	if (InvenButton)
	{
		// 버튼 초기화
		InvenButton->Init(FText::FromString(TEXT("인벤토리")), 10, LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Inven")));

		// 함수 이벤트 바인딩
		InvenButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerWidget::OnInvenButtonClicked);

		SetVisibilityButtons(InvenButton, false);
	}

	// 스킬 버튼 설정
	if (SkillButton)
	{
		// 버튼 초기화
		SkillButton->Init(FText::FromString(TEXT("스킬")), 10, LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Skill")));

		// 함수 이벤트 바인딩
		SkillButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerWidget::OnSkillButtonClicked);

		SetVisibilityButtons(SkillButton, false);
	}

	// 나가기 버튼 설정
	if (ExitButton)
	{
		// 버튼 초기화
		ExitButton->Init(FText::FromString(TEXT("나가기")), 10, LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Quit")));

		// 함수 이벤트 바인딩
		ExitButton->OnButtonClicked.AddDynamic(this, &UWC_PlayerWidget::OnExitButtonClicked);

		SetVisibilityButtons(ExitButton, false);
	}
}

void UWC_PlayerWidget::SetVisibilityButtons(UWC_SMSButton* button, bool bOnVisibility)
{
	if (bOnVisibility)
	{
		button->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		button->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UWC_PlayerWidget::OnMenuButtonClicked()
{
	if (!SlideIn || !SlideOut)
	{
		return;
	}

	if (!bOnMenuOpen)
	{
		// 설정 버튼
		if (SettingButton)
		{
			SetVisibilityButtons(SettingButton, true);
		}

		// 내 정보 버튼
		if (MyInfoButton)
		{
			SetVisibilityButtons(MyInfoButton, true);
		}

		// 인벤토리 버튼
		if (InvenButton)
		{
			SetVisibilityButtons(InvenButton, true);
		}

		// 스킬 버튼
		if (SkillButton)
		{
			SetVisibilityButtons(SkillButton, true);
		}

		// 나가기 버튼
		if (ExitButton)
		{
			SetVisibilityButtons(ExitButton, true);
		}

		PlayAnimation(SlideIn);
	}
	else
	{
		// X 위치를 오른쪽으로 이동
		PlayAnimation(SlideOut);

		// 애니메이션이 끝난 후 메뉴 숨기기
		FTimerHandle HideMenuHandle;
		FTimerDelegate HideDelegate;

		HideDelegate.BindLambda([&]()
		{
			// 설정 버튼
			if (SettingButton)
			{
				SetVisibilityButtons(SettingButton, false);
			}

			// 내 정보 버튼
			if (MyInfoButton)
			{
				SetVisibilityButtons(MyInfoButton, false);
			}

			// 인벤토리 버튼
			if (InvenButton)
			{
				SetVisibilityButtons(InvenButton, false);
			}

			// 스킬 버튼
			if (SkillButton)
			{
				SetVisibilityButtons(SkillButton, false);
			}

			// 나가기 버튼
			if (ExitButton)
			{
				SetVisibilityButtons(ExitButton, false);
			}
		});
	
		GetWorld()->GetTimerManager().SetTimer(HideMenuHandle, HideDelegate, SlideOut->GetEndTime(), false);
	}

	bOnMenuOpen = !bOnMenuOpen;
}

void UWC_PlayerWidget::OnSettingButtonClicked()
{
	// 설정창 열기
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetSettingWidgetVisible(true);
	}
}

void UWC_PlayerWidget::OnMyInfoButtonClicked()
{
	// 내 정보창 열기
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetMyInfoWidgetVisible(true);
	}
}

void UWC_PlayerWidget::OnInvenButtonClicked()
{
	// 인벤토리창 열기
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetInventoryWidgetVisible(true);
		WorldHUD->SetEquipmentWidgetVisible(true);
	}
}

void UWC_PlayerWidget::OnSkillButtonClicked()
{
	// 스킬 정보창 열기
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetPlayerSkillWidgetVisible(true);
	}
}

void UWC_PlayerWidget::OnExitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}