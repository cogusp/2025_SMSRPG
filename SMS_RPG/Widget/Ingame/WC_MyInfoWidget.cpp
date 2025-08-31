#include "Widget/Ingame/WC_MyInfoWidget.h"
#include "Widget/WC_SMSButton.h"
#include "Widget/WC_SMSProgressBar.h"

#include "HUD/C_WorldHUD.h"
#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Components/TextBlock.h"

void UWC_MyInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetPlayerInfo();

	// 창 닫기 버튼 초기화
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// 창 닫기 버튼에 함수 바인딩
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_MyInfoWidget::OnCloseButtonClicked);

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (OnSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, OnSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}

void UWC_MyInfoWidget::OnCloseButtonClicked()
{
	// 내 정보창 닫기
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (OffSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, OffSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}

	if (WorldHUD)
	{
		WorldHUD->SetMyInfoWidgetVisible(false);
	}
}

void UWC_MyInfoWidget::SetPlayerInfo()
{
	FCharacterInfo CharacterInfo = CastWCharacter(GetMyCharacter())->CharacterInfo;
	CharacterInfo.CharacterName = TEXT("LEE");
	CharacterInfo.Job = EPlayerJob::WARRIOR;

	if (TEXT_Name)
	{
		TEXT_Name->SetText(FText::FromString(CharacterInfo.CharacterName));
	}

	if (TEXT_Level)
	{
		TEXT_Level->SetText(FText::AsNumber(CharacterInfo.Level));
	}

	if (TEXT_Job)
	{
		switch (CharacterInfo.Job)
		{
		case EPlayerJob::NONE:
			TEXT_Job->SetText(FText::FromString(TEXT("백수")));
			break;
		case EPlayerJob::WARRIOR:
			TEXT_Job->SetText(FText::FromString(TEXT("전사")));
			break;
		case EPlayerJob::MAGE:
			TEXT_Job->SetText(FText::FromString(TEXT("마법사")));
			break;
		default:
			TEXT_Job->SetText(FText::FromString(TEXT("오류")));
			break;
		}
	}

	if (TEXT_Hp)
	{
		TEXT_Hp->SetText(FText::AsNumber(CharacterInfo.Curhp));
	}

	if (TEXT_Mp)
	{
		TEXT_Mp->SetText(FText::AsNumber(CharacterInfo.Curmp));
	}

	if (TEXT_Damage)
	{
		TEXT_Damage->SetText(FText::AsNumber(CharacterInfo.Damage));
	}

	if (TEXT_Defense)
	{
		TEXT_Defense->SetText(FText::AsNumber(CharacterInfo.Defense));
	}

	if (TEXT_Speed)
	{
		TEXT_Speed->SetText(FText::FromString("100"));
	}

	if (TEXT_CurExp)
	{
		TEXT_CurExp->SetText(FText::AsNumber(CharacterInfo.CurExp));
	}

	if (TEXT_MaxExp)
	{
		TEXT_MaxExp->SetText(FText::AsNumber(CharacterInfo.MaxExp));
	}

	if (PlayerExpBar)
	{
		PlayerExpBar->SetPercent((float)CharacterInfo.CurExp / (float)CharacterInfo.MaxExp);
	}
}