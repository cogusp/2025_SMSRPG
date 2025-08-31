#pragma once

#include "CoreMinimal.h"
#include "Data/DA_ConsumableData.h"
#include "DA_PotionData.generated.h"

UCLASS(BlueprintType)
class STRONGMETALSTONE_API UDA_PotionData : public UDA_ConsumableData
{
	GENERATED_BODY()

public:
	// 효과 수치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	float EffectValue = 0.f;

	// 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	float Duration = 0.f;

	// 재사용 대기 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	float CooldownTime = 0.f;
};