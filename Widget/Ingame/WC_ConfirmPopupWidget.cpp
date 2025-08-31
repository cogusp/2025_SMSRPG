#include "Widget/Ingame/WC_ConfirmPopupWidget.h"
#include "Widget/WC_SMSButton.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "Data/DA_ItemData.h"
#include "HUD/C_WorldHUD.h"
#include "Component/C_InventoryComponent.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Character/C_PlayerCharacter.h"

void UWC_ConfirmPopupWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 닫기 버튼 초기화
	BTN_Yes->Init(FText::FromString(TEXT("결정")), 14);
	BTN_No->Init(FText::FromString(TEXT("취소")), 14);

	// 창 닫기 버튼에 함수 바인딩
	BTN_Yes->OnButtonClicked.AddDynamic(this, &UWC_ConfirmPopupWidget::OnYesButtonClicked);
	BTN_No->OnButtonClicked.AddDynamic(this, &UWC_ConfirmPopupWidget::OnNoButtonClicked);

	if (!TEXT_ItemCount || !TEXT_TotalPrice) return;
	
	// 텍스트 입력 시 실시간으로 변동을 확인할 함수 바인딩
	TEXT_ItemCount->OnTextChanged.AddDynamic(this, &UWC_ConfirmPopupWidget::OnTextChangedCallback);
	// 텍스트 초기화
	TEXT_ItemCount->SetText(FText::AsNumber(1));
	TEXT_ItemCount->SetFocus();
	PrevStr = "";

	// InventoryComponent를 얻어 Delegate 바인딩
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn()))
		{
			InventoryComponent = Player->GetInventoryComponent();
		}
	}
}

void UWC_ConfirmPopupWidget::OnTextChangedCallback(const FText& Text)
{
	FString InputStr = Text.ToString();

	// 숫자만 입력했는지 확인
	if (InputStr.IsEmpty() || InputStr.IsNumeric())
	{
		PrevStr = InputStr;

		if (TEXT_TotalPrice)
		{
			if (ShopMode == EShopMode::SELL)
			{
				TEXT_TotalPrice->SetText(FText::AsNumber(FCString::Atoi(*InputStr) * ItemData->ItemData.Price * 70 / 100));
			}
			else if (ShopMode == EShopMode::BUY)
			{
				TEXT_TotalPrice->SetText(FText::AsNumber(FCString::Atoi(*InputStr) * ItemData->ItemData.Price));
			}
		}
	}
	// 숫자가 아닌 경우 이전 값으로 되돌리기
	else
	{
		if (TEXT_ItemCount)
		{
			TEXT_ItemCount->SetText(FText::FromString(PrevStr));
		}
	}
}

void UWC_ConfirmPopupWidget::OnYesButtonClicked()
{
	if (!InventoryComponent) return;
	
	// 플레이어에게 변동을 줄 금액
	int32 ItemGold = 0;

	// 수량을 버튼 누른 시점의 텍스트를 직접 가져옴
	FString InputText = TEXT_ItemCount->GetText().ToString();
	int32 CountToSell = FCString::Atoi(*InputText);

	// 입력한 수량이 0보다 적은 경우
	if (CountToSell <= 0)
	{
		if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			WorldHUD->SetPopupWidgetVisible(true);
			WorldHUD->SetNoticeText(FText::FromString(TEXT("1개 이상의 수량을 입력하세요.")));
		}
		return;
	}

	if (ShopMode == EShopMode::SELL)
	{
		// 플레이어가 가진 아이템 수가 팔려는 아이템 수보다 적은 경우 알림창
		if (!(InventoryComponent->RemoveItem(ItemData, CountToSell)))
		{
			if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				WorldHUD->SetPopupWidgetVisible(true);
				WorldHUD->SetNoticeText(FText::FromString(TEXT("아이템이 부족합니다!")));
			}
			return;
		}

		// 판매
		ItemGold = ItemData->ItemData.Price * CountToSell * 70 / 100;

		AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
		AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
		if (!WorldController)return;

		if (SellSound)// 인벤토리 소리
		{
			UGameplayStatics::PlaySoundAtLocation(this, SellSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
		}
	}
	else if (ShopMode == EShopMode::BUY)
	{
		// 플레이어가 돈이 부족한 경우 알림창
		if (ItemData->ItemData.Price * CountToSell > InventoryComponent->GetGold())
		{
			if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
			{
				WorldHUD->SetPopupWidgetVisible(true);
				WorldHUD->SetNoticeText(FText::FromString(TEXT("골드가 부족합니다!")));
			}
			return;
		}

		// 구매
		ItemGold = -(ItemData->ItemData.Price * CountToSell);
		InventoryComponent->AddItem(ItemData, CountToSell);

		AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
		AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
		if (!WorldController)return;

		if (BuySound)// 인벤토리 소리
		{
			UGameplayStatics::PlaySoundAtLocation(this, BuySound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
		}
	}

	// 위젯 다 지우기
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetConfirmPopupWidgetVisible(false);
		WorldHUD->SetShopConfirmWidgetVisible(false);
	}

	// 플레이어 골드 갱신
	InventoryComponent->AddGold(ItemGold);
}

void UWC_ConfirmPopupWidget::OnNoButtonClicked()
{
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
		AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
		if (!WorldController)return;

		if (CancelSound)// 인벤토리 소리
		{
			UGameplayStatics::PlaySoundAtLocation(this, CancelSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
		}
		WorldHUD->SetConfirmPopupWidgetVisible(false);
		WorldHUD->SetShopConfirmWidgetVisible(false);
	}
}

void UWC_ConfirmPopupWidget::SetItemData(UDA_ItemData* InItemData, EShopMode InShopMode)
{
	if (!InItemData) return;

	ItemData = InItemData;
	ShopMode = InShopMode;

	if (ShopMode == EShopMode::SELL)
	{
		TEXT_TotalPrice->SetText(FText::AsNumber(ItemData->ItemData.Price * 70 / 100));
	}
	else if (ShopMode == EShopMode::BUY)
	{
		TEXT_TotalPrice->SetText(FText::AsNumber(ItemData->ItemData.Price));
	}
}