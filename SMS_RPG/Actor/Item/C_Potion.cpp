#include "Actor/Item/C_Potion.h"
#include "Data/DA_PotionData.h"

AC_Potion::AC_Potion()
{
}

void AC_Potion::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	Super::SetItemData(InData, InType);

	// DA_Potion으로 형변환
	ItemData = Cast<UDA_PotionData>(InData);
	
	if (!ItemData) return;

	ItemData->ItemType = InType;

	// Mesh, Icon 등 설정
	Mesh->SetStaticMesh(ItemData->Mesh);
}

void AC_Potion::OnPickup(AC_PlayerCharacter* Player)
{
	Super::OnPickup(Player);
}

void AC_Potion::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemData != nullptr) SetItemData(ItemData, EItemType::CONSUMABLE);  // 이걸 통해 Mesh 세팅 등 처리
}

void AC_Potion::OnUse()
{
/// 포션 사용이랑 장비 장착 스탯 만드셈
}