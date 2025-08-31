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

	// Replicate ������� ���
	SetIsReplicatedByDefault(true);

	// �κ��丮 ������ ũ�� �ʱ�ȭ
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
	// SlotIndex�� ��ȿ���� ������ ��ȯ
	if (!EquipmentSlots.IsValidIndex(SlotIndex))
	{
		return false;
	}

	auto& Slot = EquipmentSlots[SlotIndex];

	// ������� ������ ������ �ִ� �������� Ŭ ��� ��ȯ
	if (Slot.ItemQuantity < Count)
	{
		return false;
	}

	// ���� ����
	Slot.ItemQuantity -= Count;

	// ������ �ִ� ������ 0�� ���� ���
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
	// SlotIndex�� ��ȿ���� ������ ��ȯ
	if (!EquipmentSlots.IsValidIndex(SlotIndex)) return;
	
	// �������� ����ִٸ� ��ȯ
	if (EquipmentSlots[SlotIndex].IsEmpty()) return;

	UDA_PotionData* Potion = Cast<UDA_PotionData>(EquipmentSlots[SlotIndex].ItemData);
	AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner());

	// ����Ϸ��� �������� ������ �ƴϰų� ĳ���Ͱ� ���ٸ� ��ȯ
	if (!Potion || !Character) return;

	Character->ServerUpdateHP(Potion->EffectValue);
	//CastWHUD(GetMyHUD())->UpdateAllUI();

	RemoveItem(SlotIndex);
}

void UC_EquipmentComponent::ServerUpdateEquipmentSlot_Implementation(int32 Index, FInventorySlot InItemData)
{
	// ������ �ƴ� ��� ��ȯ
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;

	// �ε��� ���� ��ȿ���� ���� ��� ��ȯ
	if (!EquipmentSlots.IsValidIndex(Index)) return;

	// ���� ���� ���� ��� ���� ���
	if (!EquipmentSlots[Index].IsEmpty())
	{
		TakeOff(Index);
		EquipmentSlots[Index] = InItemData;

		// Ż�� �� �����ϴ� ���
		if (!InItemData.IsEmpty())
		{
			TakeOn(Index);
		}
	}
	// ���� ���� ���� ��� ���� ���
	else
	{
		EquipmentSlots[Index] = InItemData;
		TakeOn(Index);
	}
}

void UC_EquipmentComponent::OnRep_EquipmentSlots()
{
	// UI ������Ʈ
	OnEquipmentUpdate.Broadcast();
}

void UC_EquipmentComponent::TakeOn(int32 index)
{
	if (AC_PlayerCharacter* Character = Cast<AC_PlayerCharacter>(GetOwner()))
	{
		// ���� ���, ���� ����
		if (UDA_ArmorData* InArmorData = Cast<UDA_ArmorData>(EquipmentSlots[index].ItemData))
		{
			Character->CharacterInfo.Defense += InArmorData->DefensePoint;
		}
		// ������ ���, ���ݷ� ����
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
		// ���� ���, ���� ����
		if (UDA_ArmorData* InArmorData = Cast<UDA_ArmorData>(EquipmentSlots[index].ItemData))
		{
			Character->CharacterInfo.Defense -= InArmorData->DefensePoint;
		}
		// ������ ���, ���ݷ� ����
		else if (UDA_WeaponData* InWeaponData = Cast<UDA_WeaponData>(EquipmentSlots[index].ItemData))
		{
			Character->CharacterInfo.Damage -= InWeaponData->AttackPoint;
		}

		Character->ServerUpdate(Character->CharacterInfo);
	}
}