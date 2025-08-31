#include "Actor/Item/C_Helmet.h"
#include "Data/DA_ArmorData.h"

AC_Helmet::AC_Helmet()
{
}

void AC_Helmet::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	Super::SetItemData(InData, InType);

	// DA_Potion으로 형변환
	ItemData = Cast<UDA_ArmorData>(InData);

	if (!ItemData) return;

	ItemData->ItemType = InType;

	// Mesh, Icon 등 설정
	Mesh->SetStaticMesh(ItemData->Mesh);
}

void AC_Helmet::OnPickup(AC_PlayerCharacter* Player)
{
	Super::OnPickup(Player);
}

void AC_Helmet::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemData != nullptr) SetItemData(ItemData, EItemType::HELMET);  // 이걸 통해 Mesh 세팅 등 처리
}