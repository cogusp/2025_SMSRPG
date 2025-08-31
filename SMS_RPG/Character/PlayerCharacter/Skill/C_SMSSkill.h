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
	FName SkillName;	// ��ų �̸�

	// ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	ESkillType SkillType; //��ųŸ��

	// ��ų ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	FText Description;

	// ��ų�� ���� �� �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Set Skill Info")
	int32 UnlockedLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float CoolTime; //��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CoolTime")
	float DemandedMana; //�ʿ丶��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaxCombo")
	int32 MaxCombo = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* SkillMontage; //��ų ��Ÿ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage")
	class UAnimMontage* ExtraSkillMontage; //��ų ��Ÿ��(�߰�)
};