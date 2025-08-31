#include "Widget/Ingame/WC_ShopConfirmWidget.h"
#include "Widget/WC_SMSButton.h"

#include "Data/DA_ItemData.h"
#include "HUD/C_WorldHUD.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Character/C_PlayerCharacter.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWC_ShopConfirmWidget::NativeConstruct()
{
	// 창 닫기 버튼 초기화
	BTN_Yes->Init(FText::FromString(TEXT("결정")), 16);
	BTN_No->Init(FText::FromString(TEXT("취소")), 16);

	// 창 닫기 버튼에 함수 바인딩
	BTN_Yes->OnButtonClicked.AddDynamic(this, &UWC_ShopConfirmWidget::OnYesButtonClicked);
	BTN_No->OnButtonClicked.AddDynamic(this, &UWC_ShopConfirmWidget::OnNoButtonClicked);
}

void UWC_ShopConfirmWidget::SetWidgetFromData(UDA_ItemData* InItemData, EShopMode InShopMode)
{
	if (!InItemData) return;

	if (InItemData->Icon)
		ItemImage->SetBrushFromTexture(InItemData->Icon);

	ItemName->SetText(FText::FromName(InItemData->ItemData.Name));
	ItemPrice->SetText(FText::AsNumber(InItemData->ItemData.Price));
	ItemDescription->SetText(InItemData->ItemData.Description);

	ItemData = InItemData;
	ShopMode = InShopMode;
}

void UWC_ShopConfirmWidget::OnYesButtonClicked()
{
	// 수량창

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (ButtonSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}

	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetConfirmPopupWidgetVisible(true);
		WorldHUD->SetItemData(ItemData, ShopMode);
	}
}

void UWC_ShopConfirmWidget::OnNoButtonClicked()
{
	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (ButtonSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, ButtonSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}

	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetShopConfirmWidgetVisible(false);
	}
}