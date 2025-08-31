#include "Widget/Ingame/WC_PopupWidget.h"
#include "Widget/WC_SMSButton.h"

#include "Components/TextBlock.h"
#include "HUD/C_WorldHUD.h"
#include "PlayerController/C_WorldPlayerController.h"

void UWC_PopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 닫기 버튼 초기화
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	BTN_Check->Init(FText::FromString(TEXT("확 인")), 14);
	// 창 닫기 버튼에 함수 바인딩
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_PopupWidget::OnCloseButtonClicked);
	BTN_Check->OnButtonClicked.AddDynamic(this, &UWC_PopupWidget::OnCloseButtonClicked);

	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (WarningSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, WarningSound, WorldController->GetPawn()->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}

void UWC_PopupWidget::SetNoticeText(const FText& text)
{
	TEXT_Log->SetText(text);
}

void UWC_PopupWidget::OnCloseButtonClicked()
{
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetPopupWidgetVisible(false);
	}
}