#include "Component/C_SkillSystemComponent.h"
#include "Component/C_ParticleSystemComponent.h"

#include "Character/C_PlayerCharacter.h"
#include "Character/Skill/C_SMSSkill.h"
#include "Character/Skill/C_SMSSkillINS.h"
#include "Character/Skill/DA_SkillList.h"

#include "PlayerController/C_WorldPlayerController.h"
#include "HUD/C_WorldHUD.h"
#include "Net/UnrealNetwork.h"

UC_SkillSystemComponent::UC_SkillSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);

	for (int32 i = 0; i < 6; ++i)
	{
		ESkillInput InSkillInput = static_cast<ESkillInput>(static_cast<uint8>(ESkillInput::SKILL1) + i);

		SlotSkills.Add(InSkillInput, FSkillData());
	}

	EquipmentSkills.SetNum(6);
}

void UC_SkillSystemComponent::InitSkill(AC_SMSSkillINS*& Skill, TSubclassOf<AC_SMSSkillINS> SkillClass)
{
	if (!SkillClass) return;

	//Skill = NewObject<AC_SMSSkillINS>(GetOwner(), SkillClass);
	if (!GetOwner()->HasAuthority()) return;

	AC_SMSSkillINS* NewSkill = GetOwner()->GetWorld()->SpawnActor<AC_SMSSkillINS>(SkillClass);
	NewSkill->Init();
	Skill = NewSkill;
}

void UC_SkillSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UC_SkillSystemComponent, Skill1);
	DOREPLIFETIME(UC_SkillSystemComponent, Skill2);
	DOREPLIFETIME(UC_SkillSystemComponent, Skill3);
	DOREPLIFETIME(UC_SkillSystemComponent, Skill4);
	DOREPLIFETIME(UC_SkillSystemComponent, SkillQ);
	DOREPLIFETIME(UC_SkillSystemComponent, SkillE);
	DOREPLIFETIME(UC_SkillSystemComponent, Attack);
	DOREPLIFETIME(UC_SkillSystemComponent, Dash);
	DOREPLIFETIME(UC_SkillSystemComponent, Hold);
	DOREPLIFETIME(UC_SkillSystemComponent, Escape);
	DOREPLIFETIME(UC_SkillSystemComponent, CurSkill);

	DOREPLIFETIME(UC_SkillSystemComponent, EquipmentSkills);
}

void UC_SkillSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	InitSkill(Skill1, Skill1Class);
	InitSkill(Skill2, Skill2Class);
	InitSkill(Skill3, Skill3Class);
	InitSkill(Skill4, Skill4Class);
	InitSkill(SkillQ, SkillQClass);
	InitSkill(SkillE, SkillEClass);
	InitSkill(Dash, DashClass);
	InitSkill(Attack, AttackClass);
	InitSkill(Hold, HoldClass);
	InitSkill(Escape, EscapeClass);
}

void UC_SkillSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UC_SkillSystemComponent::PlaySkill(AC_SMSSkillINS* Skill)
{
	if (!Skill) return;

	if (!Cast<AC_SMSCharacter>(GetOwner())) return;

	CurSkill = Skill;
	if (Cast<AC_PlayerCharacter>(GetOwner()))
	{
		CurOverHeat = Cast<AC_PlayerCharacter>(GetOwner())->ParticleSystemComponent->OverHeat;
		if (Skill->DemandedMana > Cast<AC_PlayerCharacter>(GetOwner())->CharacterInfo.Curmp) return;
	}

	Cast<AC_SMSCharacter>(GetOwner())->ServerSetCurSkill(Skill);

	Skill->StartOnce(Cast<AC_SMSCharacter>(GetOwner()));
}

void UC_SkillSystemComponent::HoldSkill(AC_SMSSkillINS* Skill)
{
	if (!Skill) return;

	if (!Cast<AC_SMSCharacter>(GetOwner()))return;
	CurSkill = Skill;
	//CastWCharacter(Cast<AC_SMSCharacter>(GetOwner()))->ServerSetCurSkill(Skill);

	switch (Skill->SkillAsset->SkillType)
	{
	case ESkillType::ONCE:
		break;
	case ESkillType::HOLD:
		Skill->Holding(CastWCharacter(GetOwner()));
		break;
	default:
		break;
	}
}

void UC_SkillSystemComponent::EndSkill(AC_SMSSkillINS* Skill)
{
	if (!Skill) return;

	switch (Skill->SkillAsset->SkillType)
	{
	case ESkillType::ONCE:
		break;
	case ESkillType::HOLD:
		Skill->EndHold(CastWCharacter(GetOwner()));
		break;
	default:
		break;
	}
}

// ----------------------------- 새로 만든 스킬 사용 함수 ---------------------------------
void UC_SkillSystemComponent::StartSkills(ESkillInput InSkillInput)
{

	if (!SlotSkills[InSkillInput].SkillData) return;

	if (!Cast<AC_SMSCharacter>(GetOwner())) return;

	// 파티클 생성
	if (Cast<AC_PlayerCharacter>(GetOwner()))
	{
		CurOverHeat = Cast<AC_PlayerCharacter>(GetOwner())->ParticleSystemComponent->OverHeat;
		if (SlotSkills[InSkillInput].SkillInstance->DemandedMana > Cast<AC_PlayerCharacter>(GetOwner())->CharacterInfo.Curmp) return;
	}

	CurSkill = SlotSkills[InSkillInput].SkillInstance;

	// 서버에 현재 스킬 설정 및 몽타주 재생
	Cast<AC_SMSCharacter>(GetOwner())->ServerSetCurSkill(CurSkill);
	CurSkill->StartOnce(Cast<AC_SMSCharacter>(GetOwner()));
}

void UC_SkillSystemComponent::HoldSkills(ESkillInput InSkillInput)
{
	if (!SlotSkills[InSkillInput].SkillData) return;
	CurSkill = SlotSkills[InSkillInput].SkillInstance;
	if (!CurSkill)return;
	if (!Cast<AC_SMSCharacter>(GetOwner())) return;

	switch (CurSkill->SkillAsset->SkillType)
	{
	case ESkillType::ONCE:
		break;
	case ESkillType::HOLD:
		CurSkill->Holding(CastWCharacter(GetOwner()));
		break;
	default:
		break;
	}
}

void UC_SkillSystemComponent::EndSkills(ESkillInput InSkillInput)
{
	if (!SlotSkills[InSkillInput].SkillData) return;
	if (!CurSkill)return;
	switch (CurSkill->SkillAsset->SkillType)
	{
	case ESkillType::ONCE:
		break;
	case ESkillType::HOLD:
		CurSkill->EndHold(CastWCharacter(GetOwner()));
		break;
	default:
		break;
	}
}

void UC_SkillSystemComponent::InitSkills(const FSkillList& InSkillList)
{
	FCharacterInfo CharacterInfo = CastWCharacter(GetOwner())->CharacterInfo;

	// 스킬 리스트 내의 스킬 순회
	for (FSkillData SkillAsset : InSkillList.Skills)
	{
		if (!SkillAsset.SkillData) continue;

		// 레벨 확인
		if (SkillAsset.SkillData->UnlockedLevel <= CharacterInfo.Level)
		{
			// 이미 보유한 스킬인지 확인
			bool bHasOwned = OwnedSkills.ContainsByPredicate(
				[&](const FSkillData& ExistingSkill)
				{
					return ExistingSkill.SkillData && ExistingSkill.SkillData->SkillName == SkillAsset.SkillData->SkillName;
				}
			);

			if (!bHasOwned)
			{
				OwnedSkills.Add(SkillAsset);
			}
		}
	}

	// 시작 시, 기본 스킬 적용
	
	for (int32 i = 0; i < 6; ++i)
	{
		ESkillInput InSkillInput = static_cast<ESkillInput>(static_cast<uint8>(ESkillInput::SKILL1) + i);

		ServerRegisterSkillSlot(InSkillInput, OwnedSkills[i]);
	}

	OnSkillSlotUpdate.Broadcast();
}

void UC_SkillSystemComponent::AddSkill(FSkillData InSkillData)
{
	if (!InSkillData.SkillData) return;

	// 이미 보유한 스킬인지 확인
	bool bHasOwned = OwnedSkills.ContainsByPredicate(
		[&](const FSkillData& ExistingSkill)
		{
			return ExistingSkill.SkillData && ExistingSkill.SkillData->SkillName == InSkillData.SkillData->SkillName;
		}
	);

	if (bHasOwned) return;

	OwnedSkills.Add(InSkillData);

	OnSkillSlotUpdate.Broadcast();
}

void UC_SkillSystemComponent::ServerRegisterSkillSlot_Implementation(ESkillInput InSkillInput, FSkillData InSkillData)
{
	if (!GetOwner()->HasAuthority()) return;
	if (!IsValid(this)) return;
	if (!InSkillData.SkillData) return;

	// 입력 키에 맞는 스킬 등록
	FSkillData NewSkillData = InSkillData;
	NewSkillData.SkillInstance = GetWorld()->SpawnActor<AC_SMSSkillINS>(InSkillData.SkillInstanceClass);
	SlotSkills.Add(InSkillInput, NewSkillData);

	// 클라이언트로 전파할 데이터 저장
	EquipmentSkills[static_cast<uint8>(InSkillInput) - 1] = InSkillData;

	// 서버에서만 호출되는 델리게이트
	OnSkillSlotUpdate.Broadcast();
	OnSkillRegister.Broadcast();
}

void UC_SkillSystemComponent::OnRep_EquipmentSkills()
{
	// 클라이언트에서 SlotSkills 재구성
	SlotSkills.Empty();

	for (int32 i = 0; i < EquipmentSkills.Num(); ++i)
	{
		if (EquipmentSkills[i].SkillData == nullptr) continue;

		ESkillInput SkillInput = static_cast<ESkillInput>(i + 1); // SKILL1 == 1
		FSkillData ReconstructedData;
		ReconstructedData = EquipmentSkills[i];
		ReconstructedData.SkillInstance = GetWorld()->SpawnActor<AC_SMSSkillINS>(ReconstructedData.SkillInstanceClass);
		
		SlotSkills.Add(SkillInput, ReconstructedData);
	}

	// UI 갱신 (클라이언트)
	OnSkillSlotUpdate.Broadcast();
	OnSkillRegister.Broadcast();
}

bool UC_SkillSystemComponent::CheckSkillSlot(FSkillData InSkillData)
{
	if (!IsValid(this)) return false;
	if (!InSkillData.SkillData) return false;

	for (const auto s : SlotSkills)
	{
		if (s.Value.SkillData == InSkillData.SkillData) return false;
	}

	return true;
}
