#pragma once

#include "CoreMinimal.h"
#include "Data/DA_ItemData.h"
#include "DA_ArmorData.generated.h"

UCLASS(BlueprintType)
class STRONGMETALSTONE_API UDA_ArmorData : public UDA_ItemData
{
	GENERATED_BODY()

public:
	// ¹æ¾î·Â
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Armor")
	float DefensePoint = 0.f;
};
