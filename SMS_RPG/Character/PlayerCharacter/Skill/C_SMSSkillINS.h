#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_SMSSkill.h"
#include "C_SMSSkillINS.generated.h"

class AC_PlayerCharacter;
class AC_SMSCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerCooldown, AC_SMSSkillINS*, SkillInstance);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStartCooldown, AC_SMSSkillINS*, SkillInstance);

UCLASS()

class STRONGMETALSTONE_API AC_SMSSkillINS : public AActor
{
	GENERATED_BODY()
	
private:

public:	
	AC_SMSSkillINS();
	virtual void BeginPlay() override;
	void Init();

	virtual void StartOnce(AC_SMSCharacter* Character); //��������
	virtual void Holding(AC_SMSCharacter* Character); //��������
	virtual void EndHold(AC_SMSCharacter* Character); //������

	void DelayCoolTime();
	void CoolTimeInit();
	void SkillComboReset();
	void AttackIMCReset();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillAsset)
	UC_SMSSkill* SkillAsset;

	UPROPERTY()
	int32 SkillCombo = 0; // ���� �޺�

	bool OnCool = true; // ��ų ���� �Ұ�
	bool bOnStart = false;

	FTimerHandle CoolTimerHandle;
	FTimerHandle ComboTimerHandle;
	FTimerHandle AttackIMCTimerHandle;

	float CoolTime;

	float DemandedMana;

	// ��Ÿ�� UI�� ���� ��������Ʈ
	UPROPERTY(BlueprintAssignable)
	FOnTriggerCooldown OnTriggerCooldown;

	UPROPERTY(BlueprintAssignable)
	FOnStartCooldown OnStartCooldown;
};