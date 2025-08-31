#include "Actor/Item/C_Armor.h"
#include "Data/DA_ArmorData.h"

AC_Armor::AC_Armor()
{
}

void AC_Armor::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	Super::SetItemData(InData, InType);

	// DA_Potion���� ����ȯ
	ItemData = Cast<UDA_ArmorData>(InData);

	if (!ItemData) return;

	ItemData->ItemType = InType;

	// Mesh, Icon �� ����
	Mesh->SetStaticMesh(ItemData->Mesh);
}

void AC_Armor::OnPickup(AC_PlayerCharacter* Player)
{
	Super::OnPickup(Player);
}

void AC_Armor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemData != nullptr) SetItemData(ItemData, EItemType::WEAPON);  // �̰� ���� Mesh ���� �� ó��
}