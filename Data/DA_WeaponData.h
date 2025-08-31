#pragma once

#include "CoreMinimal.h"
#include "Data/DA_ItemData.h"
#include "DA_WeaponData.generated.h"

UCLASS(BlueprintType)
class STRONGMETALSTONE_API UDA_WeaponData : public UDA_ItemData
{
	GENERATED_BODY()
	
public:
	// °ø°Ý·Â
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float AttackPoint = 0.f;
};
