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

	// Replicate 대상으로 등록
	SetIsReplicatedByDefault(true);

	// 인벤토리 슬롯의 크기 초기화
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
	// 새로운 아이템이 없거나 개수가 0이면 반환
	if (!NewItem || Count <= 0) return false;

	// 소비 아이템인 경우
	if (NewItem->ItemType == EItemType::CONSUMABLE)
	{
		// 인덱스를 검사하기 위한 변수
		int32 idx = -1;

		// 이미 있는 경우 찾기
		for (auto& Slot : InventorySlots)
		{
			++idx;

			if (Slot.IsEmpty()) continue;

			// 같은 아이템일 경우
			if (Slot.ItemData == NewItem)
			{
				// 최대 수량보다 적을 경우 추가
				if (Slot.ItemQuantity < MaxItemQuantity)
				{
					// 현재 수량과 들어온 수량의 합이 최대 수량보다 작거나 같은 경우
					if (MaxItemQuantity >= (Slot.ItemQuantity + Count))
					{
						// 기존 Slot의 개수 증가
						Slot.ItemQuantity += Count;

						// 서버에 데이터 적용
						ServerUpdateInventorySlot(idx, Slot);
						OnInventoryUpdate.Broadcast();

						return true;
					}
					// 현재 수량과 들어온 수량의 합이 최대 수량보다 큰 경우
					else
					{
						// 들어온 개수
						Count += (Slot.ItemQuantity - MaxItemQuantity);
						Slot.ItemQuantity = MaxItemQuantity;

						// 서버에 데이터 적용
						ServerUpdateInventorySlot(idx, Slot);
					}
				}
			}
		}

		idx = -1;

		// 새로 넣는 경우
		for (auto& Slot : InventorySlots)
		{
			++idx;

			if (!Slot.IsEmpty()) continue;
			
			Slot.ItemData = NewItem;

			// 새로 넣으려는 개수가 최대보다 작거나 같은 경우
			if (Count <= MaxItemQuantity)
			{
				Slot.ItemQuantity = Count;
				
				// 서버에 데이터 적용
				ServerUpdateInventorySlot(idx, Slot);
				OnInventoryUpdate.Broadcast();

				return true;
			}

			// 새로 넣으려는 개수가 최대보다 큰 경우
			Slot.ItemQuantity = MaxItemQuantity;
			Count -= MaxItemQuantity;

			// 서버에 데이터 적용
			ServerUpdateInventorySlot(idx, Slot);
		}
	}
	// 소비 아이템이 아닌 경우
	else
	{
		int32 idx = -1;

		for (auto& Slot : InventorySlots)
		{
			++idx;

			if (!Slot.IsEmpty()) continue;
			
			// 슬롯에 아이템 정보 저장
			Slot.ItemData = NewItem;
			Slot.ItemQuantity = 1;

			// 서버에 데이터 적용
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
	// 인벤토리가 가득찬 경우
	return false;
}

bool UC_InventoryComponent::RemoveItem(UDA_ItemData* NewItem, int32 Count)
{
	if (!NewItem || Count <= 0) return false;

	int32 RemainingToRemove = Count;

	// 1. 아이템이 존재하는 슬롯들 전체를 검사
	for (int32 i = 0; i < InventorySize && RemainingToRemove > 0; ++i)
	{
		FInventorySlot& Slot = InventorySlots[i];

		if (Slot.IsEmpty()) continue;

		// 같은 아이템인지 확인
		if (Slot.ItemData == NewItem)
		{
			// 제거해야 하는 수가 더 큰 경우
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

	// 전부 제거하지 못했으면 실패
	if (RemainingToRemove > 0)
	{
		return false;
	}

	// 2. 실제로 삭제
	for (int32 i = 0; i < InventorySize && Count > 0; ++i)
	{
		FInventorySlot& Slot = InventorySlots[i];

		if (Slot.IsEmpty()) continue;

		// 같은 아이템인지 확인
		if (Slot.ItemData == NewItem)
		{
			// 제거해야 하는 수가 더 큰 경우
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

			// 만약 수량이 0이 되면 슬롯 초기화
			if (Slot.ItemQuantity <= 0)
			{
				Slot.ItemData = nullptr;
			}

			// 서버에 데이터 적용
			ServerUpdateInventorySlot(i, Slot);
		}
	}

	OnInventoryUpdate.Broadcast();
	return true;
}

void UC_InventoryComponent::UseItem(FInventorySlot NewSlot)
{
	// 새로 들어온 아이템이 없다면 반환
	if (!NewSlot.ItemData) return;

	// 캐릭터가 없다면 반환
	AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner());
	if (!Character) return;

	// 포션이면 사용
	if (NewSlot.ItemData->ItemType == EItemType::CONSUMABLE)
	{
		if (UDA_PotionData* Potion = Cast<UDA_PotionData>(NewSlot.ItemData))
		{
			Character->ServerUpdateHP(Potion->EffectValue);
			//CastWHUD(GetMyHUD())->UpdateAllUI();
		}
	}
	// 장비면 장착
	else if (NewSlot.ItemData->ItemType != EItemType::NONE)
	{
		if (UC_EquipmentComponent* EquipmentComponent = Character->GetEquipmentComponent())
		{
			// 장착된 장비가 있을 경우
			if (EquipmentComponent->GetInEquipmentData(GetEquipmentIndex(NewSlot.ItemData->ItemType)).ItemData)
			{
				AddItem(EquipmentComponent->GetInEquipmentData(GetEquipmentIndex(NewSlot.ItemData->ItemType)).ItemData);
			}

			EquipmentComponent->ServerUpdateEquipmentSlot(GetEquipmentIndex(NewSlot.ItemData->ItemType), NewSlot);
		}
	}

	// 아이템 삭제
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

	// Delegate 호출로 UI 갱신
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