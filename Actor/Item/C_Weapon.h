#pragma once

#include "CoreMinimal.h"
#include "Actor/Item/C_Item.h"
#include "C_Weapon.generated.h"

UCLASS()
class STRONGMETALSTONE_API AC_Weapon : public AC_Item
{
	GENERATED_BODY()

public:
	AC_Weapon();

	virtual void SetItemData(UDA_ItemData* InData, EItemType InType) override;
	virtual const UDA_ItemData* GetItemData() const override { return ItemData; }

	// 플레이어가 아이템을 줍는 함수
	virtual void OnPickup(class AC_PlayerCharacter* Player) override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
