bool UWC_ItemSlot::DropSlot(UWC_ItemSlot* OtherSlot)
{
	if (!OtherSlot || !OtherSlot->SlotData.ItemData) return false;
	if (!InventoryComponent || !EquipmentComponent) return false;

	// 슬롯 타입이 호환되지 않는 경우
	if (!IsItemCompatibleWithSlot(SlotType, OtherSlot->SlotData.ItemData->ItemType, OtherSlot->SlotData.ItemData->ItemSubType))
		return false;

	// 같은 소비 아이템이면 합치기
	if (SlotData.ItemData &&
		SlotData.ItemData->ItemType == EItemType::CONSUMABLE &&
		OtherSlot->SlotData.ItemData == SlotData.ItemData)
	{
		if (MergeSlot(OtherSlot)) MoveData(OtherSlot);
	}
	else
	{
		MoveData(OtherSlot);
	}

	RefreshSlot();
	OtherSlot->RefreshSlot();
	return true;
}

bool UC_InventoryComponent::AddItem(UDA_ItemData* NewItem, int32 Count)
{
	if (!NewItem || Count <= 0) return false;

	if (NewItem->ItemType == EItemType::CONSUMABLE)
	{
		// 기존 슬롯에서 추가 가능한지 확인
		for (auto& Slot : InventorySlots)
		{
			if (Slot.ItemData == NewItem && Slot.ItemQuantity < MaxItemQuantity)
			{
				int32 Addable = FMath::Min(Count, MaxItemQuantity - Slot.ItemQuantity);
				Slot.ItemQuantity += Addable;
				Count -= Addable;

				ServerUpdateInventorySlot(SlotIndex, Slot);
				if (Count <= 0) break;
			}
		}
	}

	// 남은 수량 새 슬롯에 배치
	for (auto& Slot : InventorySlots)
	{
		if (Slot.IsEmpty())
		{
			Slot.ItemData = NewItem;
			Slot.ItemQuantity = FMath::Min(Count, MaxItemQuantity);
			Count -= Slot.ItemQuantity;

			ServerUpdateInventorySlot(SlotIndex, Slot);
			if (Count <= 0) break;
		}
	}

	OnInventoryUpdate.Broadcast();
	return (Count <= 0);
}
