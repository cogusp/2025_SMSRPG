#include "Component/C_EquipmentComponent.h"
#include "Character/C_PlayerCharacter.h"
#include "HUD/C_WorldHUD.h"

#include "Data/DA_ItemData.h"
#include "Data/DA_PotionData.h"
#include "Data/DA_ArmorData.h"
#include "Data/DA_WeaponData.h"

#include "Net/UnrealNetwork.h"

UC_EquipmentComponent::UC_EquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// Replicate 대상으로 등록
	SetIsReplicatedByDefault(true);

	// 인벤토리 슬롯의 크기 초기화
	EquipmentSlots.SetNum(EquipmentSize);
}

void UC_EquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UC_EquipmentComponent, EquipmentSlots);
}

void UC_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

bool UC_EquipmentComponent::RemoveItem(int32 SlotIndex, int32 Count)
{
	// SlotIndex가 유효하지 않으면 반환
	if (!EquipmentSlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	auto& Slot = EquipmentSlots[SlotIndex];

	// 지우려는 개수가 가지고 있는 개수보다 클 경우 반환
	if (Slot.ItemQuantity < Count)
	{
		return false;
	}

	// 개수 갱신
	Slot.ItemQuantity -= Count;

	// 가지고 있는 개수가 0이 됐을 경우
	if (Slot.ItemQuantity <= 0)
	{
		Slot = FInventorySlot();
	}

	ServerUpdateEquipmentSlot(SlotIndex, Slot);

	OnEquipmentUpdate.Broadcast();
	return true;
}

void UC_EquipmentComponent::UseItem(int32 SlotIndex)
{
	// SlotIndex가 유효하지 않으면 반환
	if (!EquipmentSlots.IsValidIndex(SlotIndex)) return;
	
	// 아이템이 비어있다면 반환
	if (EquipmentSlots[SlotIndex].IsEmpty()) return;

	UDA_PotionData* Potion = Cast<UDA_PotionData>(EquipmentSlots[SlotIndex].ItemData);
	AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner());

	// 사용하려는 아이템이 포션이 아니거나 캐릭터가 없다면 반환
	if (!Potion || !Character) return;

	Character->ServerUpdateHP(Potion->EffectValue);
	//CastWHUD(GetMyHUD())->UpdateAllUI();

	RemoveItem(SlotIndex);
}

void UC_EquipmentComponent::ServerUpdateEquipmentSlot_Implementation(int32 Index, FInventorySlot InItemData)
{
	// 서버가 아닌 경우 반환
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	// 인덱스 값이 유효하지 않은 경우 반환
	if (!EquipmentSlots.IsValidIndex(Index)) return;

	// 현재 장착 중인 장비가 있을 경우
	if (!EquipmentSlots[Index].IsEmpty())
	{
		TakeOff(Index);
		EquipmentSlots[Index] = InItemData;

		// 탈착 후 장착하는 경우
		if (!InItemData.IsEmpty())
		{
			TakeOn(Index);
		}
	}
	// 현재 장착 중인 장비가 없을 경우
	else
	{
		EquipmentSlots[Index] = InItemData;
		TakeOn(Index);
	}
}

void UC_EquipmentComponent::OnRep_EquipmentSlots()
{
	// UI 업데이트
	OnEquipmentUpdate.Broadcast();
}

void UC_EquipmentComponent::TakeOn(int32 index)
{
	if (AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner()))
	{
		// 방어구일 경우, 방어력 증가
		if (UDA_ArmorData* InArmorData = Cast<UDA_ArmorData>(EquipmentSlots[index].ItemData))
		{
			Character->CharacterInfo.Defense += InArmorData->DefensePoint;
		}
		// 무기일 경우, 공격력 증가
		else if (UDA_WeaponData* InWeaponData = Cast<UDA_WeaponData>(EquipmentSlots[index].ItemData))
		{
			Character->CharacterInfo.Damage += InWeaponData->AttackPoint;
		}

		Character->ServerUpdate(Character->CharacterInfo);
	}
}

void UC_EquipmentComponent::TakeOff(int32 index)
{
	if (AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner()))
	{
		// 방어구일 경우, 방어력 감소
		if (UDA_ArmorData* InArmorData = Cast<UDA_ArmorData>(EquipmentSlots[index].ItemData))
		{
			Character->CharacterInfo.Defense -= InArmorData->DefensePoint;
		}
		// 무기일 경우, 공격력 감소
		else if (UDA_WeaponData* InWeaponData = Cast<UDA_WeaponData>(EquipmentSlots[index].ItemData))
		{
			Character->CharacterInfo.Damage -= InWeaponData->AttackPoint;
		}

		Character->ServerUpdate(Character->CharacterInfo);
	}
}