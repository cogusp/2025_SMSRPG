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
	// 창 닫기 버튼 초기화
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// 창 닫기 버튼에 함수 바인딩
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_SellAreaWidget::OnCloseButtonClicked);
}

bool UWC_SellAreaWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UC_ItemDragDropOperation* DragOperation = Cast<UC_ItemDragDropOperation>(InOperation);
	if (!DragOperation || !DragOperation->ItemSlot) return false;

	// 드래그 한 슬롯이 ItemSlot이 아니라면 반환
	UWC_ItemSlot* DraggedSlot = Cast<UWC_ItemSlot>(DragOperation->ItemSlot);
	if (!DraggedSlot) return false;

	// 판매 위젯 띄우기
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetShopConfirmWidgetVisible(true);
		WorldHUD->SetWidgetFromData(DraggedSlot->SlotData.ItemData, EShopMode::SELL);
	}

	return true;
}

void UWC_SellAreaWidget::OnCloseButtonClicked()
{
	// 상점 관련 위젯 전부 닫기
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

	// 스크롤 박스 내용물 지우기
	SellScrollBox->ClearChildren();

	const int32 ItemsPerRow = 5;											// 한 줄에 아이템 개수
	int32 TotalItems = ItemDataList->ItemList.Num();						// 전체 아이템 개수
	int32 RowCount = FMath::CeilToInt((float)TotalItems / ItemsPerRow);		// 몇번째 줄인지 카운드

	for (int32 i = 0; i < RowCount; ++i)
	{
		// 가로 박스 생성
		UHorizontalBox* RowBox = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());

		for (int32 j = 0; j < ItemsPerRow; ++j)
		{
			// 인덱스 번호 = 행 * 현재 줄 + 열
			int32 Idx = i * ItemsPerRow + j;

			// 현재 위치가 전체 아이템보다 큰 경우
			if (Idx >= TotalItems) break;

			UWC_SellItemSlot* Slots = CreateWidget<UWC_SellItemSlot>(this, SellItemSlotClass);
			Slots->SetVisibility(ESlateVisibility::Visible);
			Slots->SetSlot(ItemDataList->ItemList[Idx]);	// 아이템 데이터 바인딩

			UHorizontalBoxSlot* SlotContainer = RowBox->AddChildToHorizontalBox(Slots);
			SlotContainer->SetPadding(FMargin(4));	// 여백 설정
		}
		
		SellScrollBox->AddChild(RowBox);
	}
}