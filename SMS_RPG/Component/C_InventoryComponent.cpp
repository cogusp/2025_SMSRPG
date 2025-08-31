#include "Component/C_InventoryComponent.h"
#include "Component/C_EquipmentComponent.h"

#include "Character/C_PlayerCharacter.h"
#include "HUD/C_WorldHUD.h"

#include "Data/DA_ItemData.h"
#include "Data/DA_PotionData.h"

#include "Net/UnrealNetwork.h"

UC_InventoryComponent::UC_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Replicate ������� ���
	SetIsReplicatedByDefault(true);

	// �κ��丮 ������ ũ�� �ʱ�ȭ
	InventorySlots.SetNum(InventorySize);
}

void UC_InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UC_InventoryComponent, InventorySlots);
}

void UC_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UC_InventoryComponent::AddItem(UDA_ItemData* NewItem, int32 Count)
{
	// ���ο� �������� ���ų� ������ 0�̸� ��ȯ
	if (!NewItem || Count <= 0) return false;

	// �Һ� �������� ���
	if (NewItem->ItemType == EItemType::CONSUMABLE)
	{
		// �ε����� �˻��ϱ� ���� ����
		int32 idx = -1;

		// �̹� �ִ� ��� ã��
		for (auto& Slot : InventorySlots)
		{
			++idx;

			if (Slot.IsEmpty()) continue;

			// ���� �������� ���
			if (Slot.ItemData == NewItem)
			{
				// �ִ� �������� ���� ��� �߰�
				if (Slot.ItemQuantity < MaxItemQuantity)
				{
					// ���� ������ ���� ������ ���� �ִ� �������� �۰ų� ���� ���
					if (MaxItemQuantity >= (Slot.ItemQuantity + Count))
					{
						// ���� Slot�� ���� ����
						Slot.ItemQuantity += Count;

						// ������ ������ ����
						ServerUpdateInventorySlot(idx, Slot);
						OnInventoryUpdate.Broadcast();

						return true;
					}
					// ���� ������ ���� ������ ���� �ִ� �������� ū ���
					else
					{
						// ���� ����
						Count += (Slot.ItemQuantity - MaxItemQuantity);
						Slot.ItemQuantity = MaxItemQuantity;

						// ������ ������ ����
						ServerUpdateInventorySlot(idx, Slot);
					}
				}
			}
		}

		idx = -1;

		// ���� �ִ� ���
		for (auto& Slot : InventorySlots)
		{
			++idx;

			if (!Slot.IsEmpty()) continue;
			
			Slot.ItemData = NewItem;

			// ���� �������� ������ �ִ뺸�� �۰ų� ���� ���
			if (Count <= MaxItemQuantity)
			{
				Slot.ItemQuantity = Count;
				
				// ������ ������ ����
				ServerUpdateInventorySlot(idx, Slot);
				OnInventoryUpdate.Broadcast();

				return true;
			}

			// ���� �������� ������ �ִ뺸�� ū ���
			Slot.ItemQuantity = MaxItemQuantity;
			Count -= MaxItemQuantity;

			// ������ ������ ����
			ServerUpdateInventorySlot(idx, Slot);
		}
	}
	// �Һ� �������� �ƴ� ���
	else
	{
		int32 idx = -1;

		for (auto& Slot : InventorySlots)
		{
			++idx;

			if (!Slot.IsEmpty()) continue;
			
			// ���Կ� ������ ���� ����
			Slot.ItemData = NewItem;
			Slot.ItemQuantity = 1;

			// ������ ������ ����
			ServerUpdateInventorySlot(idx, Slot);

			if (Count > 1)
			{
				--Count;
				continue;
			}

			OnInventoryUpdate.Broadcast();
			return true;
		}
	}
	
	OnInventoryUpdate.Broadcast();
	// �κ��丮�� ������ ���
	return false;
}

bool UC_InventoryComponent::RemoveItem(UDA_ItemData* NewItem, int32 Count)
{
	if (!NewItem || Count <= 0) return false;

	int32 RemainingToRemove = Count;

	// 1. �������� �����ϴ� ���Ե� ��ü�� �˻�
	for (int32 i = 0; i < InventorySize && RemainingToRemove > 0; ++i)
	{
		FInventorySlot& Slot = InventorySlots[i];

		if (Slot.IsEmpty()) continue;

		// ���� ���������� Ȯ��
		if (Slot.ItemData == NewItem)
		{
			// �����ؾ� �ϴ� ���� �� ū ���
			if (Slot.ItemQuantity < RemainingToRemove)
			{
				RemainingToRemove -= Slot.ItemQuantity;
			}
			else
			{
				RemainingToRemove = 0;
			}
		}
	}

	// ���� �������� �������� ����
	if (RemainingToRemove > 0)
	{
		return false;
	}

	// 2. ������ ����
	for (int32 i = 0; i < InventorySize && Count > 0; ++i)
	{
		FInventorySlot& Slot = InventorySlots[i];

		if (Slot.IsEmpty()) continue;

		// ���� ���������� Ȯ��
		if (Slot.ItemData == NewItem)
		{
			// �����ؾ� �ϴ� ���� �� ū ���
			if (Slot.ItemQuantity < Count)
			{
				Count -= Slot.ItemQuantity;
				Slot.ItemQuantity = 0;
			}
			else
			{
				Slot.ItemQuantity -= Count;
				Count = 0;
			}

			// ���� ������ 0�� �Ǹ� ���� �ʱ�ȭ
			if (Slot.ItemQuantity <= 0)
			{
				Slot.ItemData = nullptr;
			}

			// ������ ������ ����
			ServerUpdateInventorySlot(i, Slot);
		}
	}

	OnInventoryUpdate.Broadcast();
	return true;
}

void UC_InventoryComponent::UseItem(FInventorySlot NewSlot)
{
	// ���� ���� �������� ���ٸ� ��ȯ
	if (!NewSlot.ItemData) return;

	// ĳ���Ͱ� ���ٸ� ��ȯ
	AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner());
	if (!Character) return;

	// �����̸� ���
	if (NewSlot.ItemData->ItemType == EItemType::CONSUMABLE)
	{
		if (UDA_PotionData* Potion = Cast<UDA_PotionData>(NewSlot.ItemData))
		{
			Character->ServerUpdateHP(Potion->EffectValue);
			//CastWHUD(GetMyHUD())->UpdateAllUI();
		}
	}
	// ���� ����
	else if (NewSlot.ItemData->ItemType != EItemType::NONE)
	{
		if (UC_EquipmentComponent* EquipmentComponent = Character->GetEquipmentComponent())
		{
			// ������ ��� ���� ���
			if (EquipmentComponent->GetInEquipmentData(GetEquipmentIndex(NewSlot.ItemData->ItemType)).ItemData)
			{
				AddItem(EquipmentComponent->GetInEquipmentData(GetEquipmentIndex(NewSlot.ItemData->ItemType)).ItemData);
			}

			EquipmentComponent->ServerUpdateEquipmentSlot(GetEquipmentIndex(NewSlot.ItemData->ItemType), NewSlot);
		}
	}

	// ������ ����
	RemoveItem(NewSlot.ItemData);
}

int32 UC_InventoryComponent::GetEquipmentIndex(EItemType InItemType)
{
	switch (InItemType)
	{
	case EItemType::HELMET:		return 0;
	case EItemType::UPPERARMOR: return 1;
	case EItemType::LOWERARMOR:	return 2;
	case EItemType::SHOES:		return 3;
	case EItemType::WEAPON:		return 4;
	default:					return -1;
	}
}

void UC_InventoryComponent::AddGold(int32 Count)
{
	Gold += Count;

	// Delegate ȣ��� UI ����
	OnInventoryUpdate.Broadcast();
}

void UC_InventoryComponent::ServerUpdateInventorySlot_Implementation(int32 Index, FInventorySlot InItemData)
{
	if (!InventorySlots.IsValidIndex(Index)) return;

	InventorySlots[Index] = InItemData;
}

void UC_InventoryComponent::OnRep_InventorySlots()
{
	OnInventoryUpdate.Broadcast();
}