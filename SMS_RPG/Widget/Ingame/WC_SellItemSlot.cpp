#include "Widget/Ingame/WC_SellItemSlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/C_InventoryComponent.h"
#include "HUD/C_WorldHUD.h"
#include "Data/DA_ItemData.h"

#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

FReply UWC_SellItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// 구매 확인창 띄우기
		OnSlotClicked();
		return FReply::Handled();		
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWC_SellItemSlot::OnSlotClicked()
{
	// 구매 확인창 띄우기
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetShopConfirmWidgetVisible(true);
		WorldHUD->SetWidgetFromData(ItemData, EShopMode::BUY);
	}
}

void UWC_SellItemSlot::SetSlot(UDA_ItemData* NewItemData)
{
	// 아이템 데이터 및 수량 설정
	ItemData = NewItemData;

	// 아이템 이미지 설정
	if (ItemData && ItemData->Icon)
	{
		ItemImage = ItemData->Icon;
		RefreshSlot();

		ItemName->SetText(FText::FromName(ItemData->ItemData.Name));
		ItemPrice->SetText(FText::AsNumber(ItemData->ItemData.Price));
	}
}

void UWC_SellItemSlot::RefreshSlot()
{
	if (!ItemImage)
	{
		// 브러시를 초기화하고 투명하게 설정
		FSlateBrush EmptyBrush;
		EmptyBrush.TintColor = FSlateColor(FLinearColor::Transparent);
		ItemSlot->SetBrush(EmptyBrush);
	}
	else
	{
		FSlateBrush EmptyBrush;
		EmptyBrush.TintColor = FSlateColor(FLinearColor::White);
		ItemSlot->SetBrush(EmptyBrush);
		ItemSlot->SetBrushFromTexture(ItemImage);
	}
}