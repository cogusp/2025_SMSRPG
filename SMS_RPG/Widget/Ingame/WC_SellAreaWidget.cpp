#include "Widget/Ingame/WC_SellAreaWidget.h"
#include "Widget/Inventory/WC_ItemSlot.h"
#include "Widget/Inventory/C_ItemDragDropOperation.h"
#include "Widget/Ingame/WC_SellItemSlot.h"
#include "Widget/WC_SMSButton.h"

#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

#include "Data/DA_ItemDataList.h"
#include "Data/DA_ItemData.h"

#include "HUD/C_WorldHUD.h"

void UWC_SellAreaWidget::NativeConstruct()
{
	// â �ݱ� ��ư �ʱ�ȭ
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// â �ݱ� ��ư�� �Լ� ���ε�
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_SellAreaWidget::OnCloseButtonClicked);
}

bool UWC_SellAreaWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UC_ItemDragDropOperation* DragOperation = Cast<UC_ItemDragDropOperation>(InOperation);
	if (!DragOperation || !DragOperation->ItemSlot) return false;

	// �巡�� �� ������ ItemSlot�� �ƴ϶�� ��ȯ
	UWC_ItemSlot* DraggedSlot = Cast<UWC_ItemSlot>(DragOperation->ItemSlot);
	if (!DraggedSlot) return false;

	// �Ǹ� ���� ����
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetShopConfirmWidgetVisible(true);
		WorldHUD->SetWidgetFromData(DraggedSlot->SlotData.ItemData, EShopMode::SELL);
	}

	return true;
}

void UWC_SellAreaWidget::OnCloseButtonClicked()
{
	// ���� ���� ���� ���� �ݱ�
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetInventoryWidgetVisible(false);
		WorldHUD->SetMerchantWidgetVisible(false);
		WorldHUD->SetSellAreaWidgetVisible(false);
	}
}

void UWC_SellAreaWidget::InitSellItemGrid(UDA_ItemDataList* ItemDataList)
{
	if (!SellItemSlotClass || !SellScrollBox || !ItemDataList) return;

	// ��ũ�� �ڽ� ���빰 �����
	SellScrollBox->ClearChildren();

	const int32 ItemsPerRow = 5;											// �� �ٿ� ������ ����
	int32 TotalItems = ItemDataList->ItemList.Num();						// ��ü ������ ����
	int32 RowCount = FMath::CeilToInt((float)TotalItems / ItemsPerRow);		// ���° ������ ī���

	for (int32 i = 0; i < RowCount; ++i)
	{
		// ���� �ڽ� ����
		UHorizontalBox* RowBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());

		for (int32 j = 0; j < ItemsPerRow; ++j)
		{
			// �ε��� ��ȣ = �� * ���� �� + ��
			int32 Idx = i * ItemsPerRow + j;

			// ���� ��ġ�� ��ü �����ۺ��� ū ���
			if (Idx >= TotalItems) break;

			UWC_SellItemSlot* Slots = CreateWidget<UWC_SellItemSlot>(this, SellItemSlotClass);
			Slots->SetVisibility(ESlateVisibility::Visible);
			Slots->SetSlot(ItemDataList->ItemList[Idx]);	// ������ ������ ���ε�

			UHorizontalBoxSlot* SlotContainer = RowBox->AddChildToHorizontalBox(Slots);
			SlotContainer->SetPadding(FMargin(4));	// ���� ����
		}
		
		SellScrollBox->AddChild(RowBox);
	}
}