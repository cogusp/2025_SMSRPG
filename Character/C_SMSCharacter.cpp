#include "Character/C_SMSCharacter.h"
#include "Character/C_Enemy.h"
#include "Character/C_PlayerCharacter.h"
#include "Character/C_Warrior.h"
#include "Character/Skill/DA_SkillList.h"

#include "Component/C_SkillSystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Effect/C_SMSEffectINS.h"
#include "Engine/DamageEvents.h"
#include "C_SMSGameInstance.h"

// Sets default values
AC_SMSCharacter::AC_SMSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkillSytemComponent = CreateDefaultSubobject<UC_SkillSystemComponent>(TEXT("SkillSystemComponent"));
}

void AC_SMSCharacter::BeginPlay()
{
	Super::BeginPlay();

	// GameInstance에서 전체 스킬 목록 가져오기
	const UC_SMSGameInstance* GI = GetGameInstance<UC_SMSGameInstance>();

	if (!GI || !GI->AllSkillList || !SkillSytemComponent) return;

	if (CharacterJob == EPlayerJob::NONE) return;

	GetWorld()->GetTimerManager().SetTimer( InitSkillTimerHandle,this, &AC_SMSCharacter::InitSkill, 0.5f, false);

}

void AC_SMSCharacter::Tick(float Deltatime)
{
	Super::Tick(Deltatime);	
}

float AC_SMSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{	
	if (Absolute) return 0;

	 Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))//단일공격일경우 처리
	{
		
	}
	else if(DamageEvent.IsOfType(FRadialDamageEvent::ClassID))//범위공격일경우 처리
	{

	}

	float hp=0;	

	if (Cast<AC_PlayerCharacter>(this))
	{
		hp = Cast<AC_PlayerCharacter>(this)->CharacterInfo.Curhp;
		float temp = (hp - DamageAmount) < 0.f ? hp : DamageAmount;
		hp = (hp - DamageAmount) < 0.f ? 0.f : hp - DamageAmount;
		DamageAmount = temp;
		Cast<AC_PlayerCharacter>(this)->CharacterInfo.Curhp = hp;
		Cast<AC_PlayerCharacter>(this)->MultiUpdate(Cast<AC_PlayerCharacter>(this)->CharacterInfo);
		Cast<AC_PlayerCharacter>(this)->MultiPartyUpdate(Cast<AC_PlayerCharacter>(this)->PartyInfo);
		Cast<AC_PlayerCharacter>(this)->ClientUpdate();
	}	
	else if (Cast<AC_Enemy>(this))
	{
		hp = Cast<AC_Enemy>(this)->EnemyInfo.Curhp;
		float temp = (hp - DamageAmount) < 0.f ? hp : DamageAmount;
		hp = (hp - DamageAmount) < 0.f ? 0.f : hp - DamageAmount;
		DamageAmount = temp;
		Cast<AC_Enemy>(this)->EnemyInfo.Curhp=hp ;
		Cast<AC_Enemy>(this)->MultiUpdate(Cast<AC_Enemy>(this)->EnemyInfo);
		if (!Cast<AC_Enemy>(this)->CharacterDamage.Contains(Cast<AC_PlayerCharacter>(DamageCauser)->CharacterInfo.CharacterName))
		{
			Cast<AC_Enemy>(this)->CharacterDamage.Add(Cast<AC_PlayerCharacter>(DamageCauser)->CharacterInfo.CharacterName, 0); // 서버에 누가 공격한건지 저장하기
		}
		Cast<AC_Enemy>(this)->CharacterDamage[Cast<AC_PlayerCharacter>(DamageCauser)->CharacterInfo.CharacterName] += DamageAmount;
		MultiCharacterDamage(DamageCauser, DamageAmount); // 멀티로도 뿌려주기
	}
	

	// 데미지만큼 체력 감소, 체력이 0 미만으로 내려가지 않게 제한
	

	//Update();

	// 체력이 0이 되면 캐릭터가 사망했다고 처리할 수 있음
	if (hp == 0.0f)
	{
		Death(Cast<AC_SMSCharacter>(DamageCauser));	// 사망 처리 	   
	}
		
	return DamageAmount;
}

void AC_SMSCharacter::ServerTakeEffect_Implementation(TSubclassOf<AC_SMSEffectINS> EffectClass)
{
	MultiTakeEffect(EffectClass);
}
void AC_SMSCharacter::MultiTakeEffect_Implementation(TSubclassOf<AC_SMSEffectINS> EffectClass)
{
	AC_SMSEffectINS* Effect = GetWorld()->SpawnActor<AC_SMSEffectINS>(EffectClass);
	TakeEffect(Effect);
}

void AC_SMSCharacter::TakeEffect(AC_SMSEffectINS* Effect)
{	
	if (!Effect) return;
	if (CurEffects.Contains(Effect->EffectAsset->EffectName))
	{
		CurEffects[Effect->EffectAsset->EffectName]->End();
	}
	
	CurEffects.Add(Effect->EffectAsset->EffectName,Effect);
	Effect->Start(this);
}

void AC_SMSCharacter::Init()
{

}

void AC_SMSCharacter::Death(AC_SMSCharacter* Killer)
{
	MultiPlayMontage(DeathMontage);
}

void AC_SMSCharacter::Update()
{
}


void AC_SMSCharacter::ServerSetCurSkill_Implementation(AC_SMSSkillINS* _Skill)
{
	SkillSytemComponent->CurSkill = _Skill;

	MultiSetCurSkill(_Skill);

}

void AC_SMSCharacter::MultiSetCurSkill_Implementation(AC_SMSSkillINS* _Skill)
{
	SkillSytemComponent->CurSkill = _Skill;
}

void AC_SMSCharacter::ServerSetEnemy_Implementation(AC_SMSCharacter* Character)
{
	Character->ClientSetEnemy(this);
}

void AC_SMSCharacter::ClientSetEnemy_Implementation(AC_SMSCharacter* Character)
{
	//캐릭터 콜리전 적으로 변경
	Character->GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void AC_SMSCharacter::ServerPlayMontage_Implementation(UAnimMontage* Montage, const FName& SectionName)
{
	if (GetCurrentMontage() == DeathMontage) return;

	MultiPlayMontage(Montage, SectionName); //멀티몽타주 실행
}

void AC_SMSCharacter::MultiPlayMontage_Implementation(UAnimMontage* Montage, const FName& SectionName)
{
	if (GetCurrentMontage() == DeathMontage) return;
	if (Montage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance(); 

		if (AnimInstance)
		{						
			AnimInstance->Montage_Play(Montage); //몽타주 실행
			AnimInstance->Montage_JumpToSection(SectionName, Montage); //몽타주 section 실행
		}
	}
}

void AC_SMSCharacter::ServerStopMontage_Implementation()
{
	MultiStopMontage();
}

void AC_SMSCharacter::MultiStopMontage_Implementation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.2f); //몽타주 정지
	}
}

void AC_SMSCharacter::MultiCharacterDamage_Implementation(AActor* _DamageCauser,float _DamageAmount)
{
	if (!Cast<AC_Enemy>(this)->CharacterDamage.Contains(Cast<AC_PlayerCharacter>(_DamageCauser)->CharacterInfo.CharacterName))
	{
		Cast<AC_Enemy>(this)->CharacterDamage.Add(Cast<AC_PlayerCharacter>(_DamageCauser)->CharacterInfo.CharacterName, 0);
	}
	Cast<AC_Enemy>(this)->CharacterDamage[Cast<AC_PlayerCharacter>(_DamageCauser)->CharacterInfo.CharacterName] += _DamageAmount;
}

void AC_SMSCharacter::ServerSetAbsolute_Implementation(bool _Absolute)
{
	Absolute = _Absolute;
}

void AC_SMSCharacter::InitSkill()
{	// 자신의 Job에 해당하는 스킬 목록 검색
	if (const FSkillList* FoundSkillList = CastWGI(GetMyGI())->AllSkillList->SkillMap.Find(CharacterJob))
	{
		SkillSytemComponent->InitSkills(*FoundSkillList);
	}
}
