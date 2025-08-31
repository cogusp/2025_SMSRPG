// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/Item/C_Feather.h"
#include "Data/DA_Feather.h"

AC_Feather::AC_Feather()
{
}

void AC_Feather::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	Super::SetItemData(InData, InType);

	// DA_Feather으로 형변환
	ItemData = Cast<UDA_Feather>(InData);

	if (!ItemData) return;

	ItemData->ItemType = InType;

	// Mesh, Icon 등 설정
	Mesh->SetStaticMesh(ItemData->Mesh);
}

void AC_Feather::OnPickup(AC_PlayerCharacter* Player)
{
	Super::OnPickup(Player);
}

void AC_Feather::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemData != nullptr) SetItemData(ItemData, EItemType::CONSUMABLE);  // 이걸 통해 Mesh 세팅 등 처리
}
