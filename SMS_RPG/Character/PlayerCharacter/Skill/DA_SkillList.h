#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "C_SMSEnum.h"
#include "C_SMSStruct.h"
#include "DA_SkillList.generated.h"

class UC_SMSSkill;

UCLASS()
class STRONGMETALSTONE_API UDA_SkillList : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EPlayerJob, FSkillList> SkillMap;
};
