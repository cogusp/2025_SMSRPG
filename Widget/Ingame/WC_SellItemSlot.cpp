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
		// ���� Ȯ��â ����
		OnSlotClicked();
		return FReply::Handled();		
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWC_SellItemSlot::OnSlotClicked()
{
	// ���� Ȯ��â ����
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetShopConfirmWidgetVisible(true);
		WorldHUD->SetWidgetFromData(ItemData, EShopMode::BUY);
	}
}

void UWC_SellItemSlot::SetSlot(UDA_ItemData* NewItemData)
{
	// ������ ������ �� ���� ����
	ItemData = NewItemData;

	// ������ �̹��� ����
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
		// �귯�ø� �ʱ�ȭ�ϰ� �����ϰ� ����
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