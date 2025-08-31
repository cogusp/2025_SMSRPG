#include "Widget/Inventory/WC_ItemSlot.h"
#include "Widget/Inventory/WC_InventoryWidget.h"

#include "Data/DA_ItemData.h"
#include "Character/C_PlayerCharacter.h"

#include "Component/C_InventoryComponent.h"
#include "Component/C_EquipmentComponent.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UWC_ItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayerPawn())
	{
		// 컴포넌트 저장
		if (AC_PlayerCharacter* Player = Cast<AC_PlayerCharacter>(GetOwningPlayerPawn()))
		{
			InventoryComponent = Player->GetInventoryComponent();
			EquipmentComponent = Player->GetEquipmentComponent();
		}
	}
}

FReply UWC_ItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 좌클릭 시 Drag를 허용하기 위해 좌클릭일 때
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// 좌클릭 확인 후 반환
		bDragDetected = false;
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		// 우클릭
		SlotRightClicked();
	}

	return UWC_Slot::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UWC_ItemSlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 드래그 중일 경우, 반환
	if (bDragDetected) return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	// 아이템 데이터가 없으면 반환
	if (!SlotData.ItemData) return FReply::Handled();

	// 좌클릭
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) SlotLeftClicked();

	return FReply::Handled();
}

void UWC_ItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	bDragDetected = true;

	UWC_Slot::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UWC_ItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return UWC_Slot::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

bool UWC_ItemSlot::DropSlot(UWC_ItemSlot* OtherSlot)
{
	// 드래그한 슬롯이 비어있을 경우 반환
	if (!OtherSlot || !OtherSlot->SlotData.ItemData) return false;

	if (!InventoryComponent || !EquipmentComponent) return false;

	// 이동할 슬롯과 드래그한 슬롯의 타입이 맞지 않는 경우 반환
	if (!IsItemCompatibleWithSlot(SlotType, OtherSlot->SlotData.ItemData->ItemType, OtherSlot->SlotData.ItemData->ItemSubType))
		return false;
	
	// 소모품일 경우, 합치기
	if (SlotData.ItemData &&
		SlotData.ItemData->ItemType == EItemType::CONSUMABLE &&
		OtherSlot->SlotData.ItemData->ItemType == EItemType::CONSUMABLE &&
		SlotData.ItemData == OtherSlot->SlotData.ItemData)
	{
		if (MergeSlot(OtherSlot))
			MoveData(OtherSlot);
	}
	else
	{
		MoveData(OtherSlot);
	}

	// UI 업데이트
	RefreshSlot();
	OtherSlot->RefreshSlot();

	return true;
}

void UWC_ItemSlot::SlotLeftClicked()
{
	// 장비 + 포션 슬롯이 아니면 반환
	if (SlotType == ESlotType::Potion1 || SlotType == ESlotType::Potion2)
	{
		if (!SlotData.ItemData) return;

		// 소비품이 아니면 반환
		if (SlotData.ItemData->ItemType != EItemType::CONSUMABLE) return;

		EquipmentComponent->UseItem(static_cast<uint8>(SlotType) - 1);
	}
}

void UWC_ItemSlot::SlotRightClicked()
{
	if (!SlotData.ItemData) return;

	if (SlotType == ESlotType::Potion1 || SlotType == ESlotType::Potion2) return;

	// 인벤토리일 경우
	if (SlotType == ESlotType::NONE)
	{
		if (!ParentWidget) return;

		// 포션일 경우
		if (SlotData.ItemData->ItemType == EItemType::CONSUMABLE)
			ParentWidget->SetShowOptionWidget(this, FText::FromString(TEXT("사 용")));
		// 장비일 경우
		else if (SlotData.ItemData->ItemType != EItemType::NONE)
			ParentWidget->SetShowOptionWidget(this, FText::FromString(TEXT("장 착")));
	}
	// 장비창일 경우
	else
	{
		InventoryComponent->AddItem(SlotData.ItemData, SlotData.ItemQuantity);
		EquipmentComponent->RemoveItem(GetEquipmentSlotIndex(SlotType));
	}
}

void UWC_ItemSlot::SetSlot(FInventorySlot InSlotData)
{
	// 아이템 데이터 및 수량 설정
	SlotData = InSlotData;

	// 아이템 이미지 설정
	if (SlotData.ItemData)
		ItemImage = SlotData.ItemData->Icon;
	else
		ItemImage = nullptr;

	RefreshSlot();
}

bool UWC_ItemSlot::MergeSlot(UWC_ItemSlot* OtherSlot)
{
	const int32 MaxQuantity = InventoryComponent->MaxItemQuantity;
	const int32 TotalQuantity = SlotData.ItemQuantity + OtherSlot->SlotData.ItemQuantity;

	// 두 개를 더한 값이 최대 값보다 작은 경우
	if (TotalQuantity <= MaxQuantity)
	{
		OtherSlot->SlotData.ItemQuantity = TotalQuantity;
		this->ClearSlot();
	}
	// 두 개를 더한 값이 최대 값보다 큰 경우
	else
	{
		const int32 Remaining = TotalQuantity - MaxQuantity;
		OtherSlot->SlotData.ItemQuantity = MaxQuantity;
		SlotData.ItemQuantity = Remaining;

		return false;
	}

	return true;
}

void UWC_ItemSlot::MoveData(UWC_ItemSlot* OtherSlot)
{
	FInventorySlot Temp = OtherSlot->SlotData;

	// OtherSlot이 인벤토리 일 때
	if (OtherSlot->SlotType == ESlotType::NONE)
	{
		// 이동하려는 슬롯이 인벤토리일 때
		if (SlotType == ESlotType::NONE)
		{
			InventoryComponent->ServerUpdateInventorySlot(OtherSlot->SlotIndex, SlotData);
			InventoryComponent->ServerUpdateInventorySlot(SlotIndex, Temp);
		}
		// 이동하려는 슬롯이 인벤토리가 아닐 때
		else
		{
			InventoryComponent->ServerUpdateInventorySlot(OtherSlot->SlotIndex, SlotData);
			EquipmentComponent->ServerUpdateEquipmentSlot(GetEquipmentSlotIndex(SlotType), Temp);
		}
	}
	// OtherSlot이 인벤토리가 아닐 때
	else
	{
		// 이동하려는 슬롯이 인벤토리일 때
		if (SlotType == ESlotType::NONE)
		{
			EquipmentComponent->ServerUpdateEquipmentSlot(GetEquipmentSlotIndex(OtherSlot->SlotType), SlotData);
			InventoryComponent->ServerUpdateInventorySlot(SlotIndex, Temp);
		}
		// 이동하려는 슬롯이 인벤토리가 아닐 때
		else
		{
			EquipmentComponent->ServerUpdateEquipmentSlot(GetEquipmentSlotIndex(SlotType), SlotData);
			EquipmentComponent->ServerUpdateEquipmentSlot(GetEquipmentSlotIndex(OtherSlot->SlotType), Temp);
		}
	}

	RefreshSlot();
	OtherSlot->RefreshSlot();

	return;
}

bool UWC_ItemSlot::IsItemCompatibleWithSlot(ESlotType InSlotType, EItemType InItemType, EItemSubType InItemSubType)
{
	switch (InSlotType)
	{
	case ESlotType::NONE:		return true;
	case ESlotType::HELMET:		return InItemType == EItemType::HELMET;
	case ESlotType::UPPERARMOR:	return InItemType == EItemType::UPPERARMOR;
	case ESlotType::LOWERARMOR:	return InItemType == EItemType::LOWERARMOR;
	case ESlotType::SHOES:		return InItemType == EItemType::SHOES;
	case ESlotType::WEAPON:		return InItemType == EItemType::WEAPON;
	case ESlotType::Potion1:
	case ESlotType::Potion2:	
		return InItemType == EItemType::CONSUMABLE && InItemSubType == EItemSubType::POTION;
	default:					return false;
	}
}

int32 UWC_ItemSlot::GetEquipmentSlotIndex(ESlotType InSlotType)
{
	switch (InSlotType)
	{
	case ESlotType::HELMET:		return 0;
	case ESlotType::UPPERARMOR:	return 1;
	case ESlotType::LOWERARMOR:	return 2;
	case ESlotType::SHOES:		return 3;
	case ESlotType::WEAPON:		return 4;
	case ESlotType::Potion1:	return 5;
	case ESlotType::Potion2:	return 6;
	default:					return -1;
	}
}

void UWC_ItemSlot::ClearSlot()
{
	// 기존 슬롯 지우기
	SlotData = FInventorySlot();

	UWC_Slot::ClearSlot();
}

void UWC_ItemSlot::RefreshSlot()
{
	// 아이템 데이터가 있을 경우
	if (SlotData.ItemData)
	{
		ItemSlot->SetBrushFromTexture(ItemImage);

		// 소모품일 경우 텍스트 세팅
		if (SlotData.ItemData->ItemType == EItemType::CONSUMABLE)
		{
			QuantityText->SetText(FText::AsNumber(SlotData.ItemQuantity));
			QuantityText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		// 소모품이 아닌 경우 텍스트 숨기기
		else
		{
			QuantityText->SetText(FText::GetEmpty());
			QuantityText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	// 아이템 데이터가 없을 경우
	else
	{
		ClearSlot();

		QuantityText->SetText(FText::GetEmpty());
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}

	UWC_Slot::RefreshSlot();
}
