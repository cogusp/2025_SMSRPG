#include "Actor/Item/C_Potion.h"
#include "Data/DA_PotionData.h"

AC_Potion::AC_Potion()
{
}

void AC_Potion::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	Super::SetItemData(InData, InType);

	// DA_Potion���� ����ȯ
	ItemData = Cast<UDA_PotionData>(InData);
	
	if (!ItemData) return;

	ItemData->ItemType = InType;

	// Mesh, Icon �� ����
	Mesh->SetStaticMesh(ItemData->Mesh);
}

void AC_Potion::OnPickup(AC_PlayerCharacter* Player)
{
	Super::OnPickup(Player);
}

void AC_Potion::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemData != nullptr) SetItemData(ItemData, EItemType::CONSUMABLE);  // �̰� ���� Mesh ���� �� ó��
}

void AC_Potion::OnUse()
{
/// ���� ����̶� ��� ���� ���� �����
}