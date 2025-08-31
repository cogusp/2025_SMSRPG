#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FrameWork.h"
#include "C_SMSSkill.generated.h"

class AC_PlayerCharacter;

UCLASS()
class STRONGMETALSTONE_API UC_SMSSkill : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	FName SkillName;	// 스킬 이름

	// 스킬 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	ESkillType SkillType; //스킬타입

	// 스킬 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	FText Description;

	// 스킬을 얻을 수 있는 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	int32 UnlockedLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float CoolTime; //쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float DemandedMana; //필요마나

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxCombo")
	int32 MaxCombo = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* SkillMontage; //스킬 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* ExtraSkillMontage; //스킬 몽타주(추가)
};