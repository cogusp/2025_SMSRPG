#include "Actor/Item/C_Weapon.h"
#include "Data/DA_WeaponData.h"

AC_Weapon::AC_Weapon()
{
}

void AC_Weapon::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	Super::SetItemData(InData, InType);

	// DA_Potion으로 형변환
	ItemData = Cast<UDA_WeaponData>(InData);

	if (!ItemData) return;

	ItemData->ItemType = InType;

	// Mesh, Icon 등 설정
	Mesh->SetStaticMesh(ItemData->Mesh);
}

void AC_Weapon::OnPickup(AC_PlayerCharacter* Player)
{
	Super::OnPickup(Player);
}

void AC_Weapon::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemData != nullptr) SetItemData(ItemData, EItemType::WEAPON);  // 이걸 통해 Mesh 세팅 등 처리
}
