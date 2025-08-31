#pragma once

#include "CoreMinimal.h"
#include "Data/DA_ConsumableData.h"
#include "DA_PotionData.generated.h"

UCLASS(BlueprintType)
class STRONGMETALSTONE_API UDA_PotionData : public UDA_ConsumableData
{
	GENERATED_BODY()

public:
	// ȿ�� ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	float EffectValue = 0.f;

	// ���� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	float Duration = 0.f;

	// ���� ��� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Potion")
	float CooldownTime = 0.f;
};