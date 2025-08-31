// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/C_SMSActorComponent.h"
#include "FrameWork.h"
#include "Net/UnrealNetwork.h"
#include "Character/Skill/C_SMSSkillINS.h"
#include "C_SkillSystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillSlotUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillRegistered);

class UC_SMSSkill;
class UDA_SkillList;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STRONGMETALSTONE_API UC_SkillSystemComponent : public UC_SMSActorComponent
{
	GENERATED_BODY()

public:	
	UC_SkillSystemComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
	void InitSkill(AC_SMSSkillINS*& Skill, TSubclassOf<AC_SMSSkillINS> SkillClass);
	
	void PlaySkill(AC_SMSSkillINS* Skill);
	
	void HoldSkill(AC_SMSSkillINS* Skill);
	
	void EndSkill(AC_SMSSkillINS* Skill);

	// ��ų ���
	void StartSkills(ESkillInput InSkillInput);
	void HoldSkills(ESkillInput InSkillInput);
	void EndSkills(ESkillInput InSkillInput);

	// ��ų �ʱ�ȭ
	UFUNCTION(BlueprintCallable)
	void InitSkills(const FSkillList& InSkillList);

	// ������ ��ų �߰��ϱ�
	UFUNCTION(BlueprintCallable)
	void AddSkill(FSkillData InSkillData);

	// ���� �����͸� ���
	UFUNCTION(Server, Reliable)
	void ServerRegisterSkillSlot(ESkillInput InSkillInput, FSkillData InSkillData);

	UFUNCTION()
	void OnRep_EquipmentSkills();

	// ���� ���� �˻�
	UFUNCTION(BlueprintCallable)
	bool CheckSkillSlot(FSkillData InSkillData);

	UFUNCTION()
	TArray<FSkillData> GetOwnedSkills() { return OwnedSkills; }
	
protected:
	virtual void BeginPlay() override;

public:
	//����� ��ų
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> Skill1Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> Skill2Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> Skill3Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> Skill4Class;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> SkillQClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> SkillEClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> DashClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> AttackClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> HoldClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AC_SMSSkillINS> EscapeClass;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Skill1;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Skill2;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Skill3;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Skill4;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* SkillQ;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* SkillE;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Dash;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Attack;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Hold;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* Escape;

	UPROPERTY(Replicated)
	AC_SMSSkillINS* CurSkill;

	EOverHeat CurOverHeat;

	////
	// ���� ��ų
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<EPlayerJob, UDA_SkillList*> SkillListMap;
	
	// ������ ��ų
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSkillData> OwnedSkills;

	// ������ ��ų
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<ESkillInput, FSkillData> SlotSkills;

	// ������ ��ų�� �����ϱ� ���� �迭
	UPROPERTY(ReplicatedUsing = OnRep_EquipmentSkills)
	TArray<FSkillData> EquipmentSkills;

	// UI�� ������Ʈ �ϱ� ���� ��������Ʈ
	UPROPERTY(BlueprintAssignable)
	FOnSkillSlotUpdated OnSkillSlotUpdate;

	// ��Ÿ�� ��������Ʈ ���ε��� ���� ��������Ʈ
	UPROPERTY(BlueprintAssignable)
	FOnSkillRegistered OnSkillRegister;
};
