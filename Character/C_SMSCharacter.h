#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FrameWork.h"
#include "C_SMSCharacter.generated.h"

class AC_SMSEffectINS;
class UDA_SkillList;
class UC_SkillSystemComponent;

UCLASS()
class STRONGMETALSTONE_API AC_SMSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AC_SMSCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

    virtual float TakeDamage //데미지를 받기
	( 
		float DamageAmount,						// 데미지 양 
		struct FDamageEvent const& DamageEvent,	// 데미지 이벤트
		class AController* EventInstigator,		// 데미지를 준 컨트롤러
		AActor* DamageCauser					// 데미지를 준 엑터
	) override; 

	UFUNCTION()
	void TakeEffect(AC_SMSEffectINS* Effect);

	virtual void Init();	// 초기화
	virtual void Death(AC_SMSCharacter* Killer);	// 죽음
	virtual void Update();	// RPC를 통해 상태를 갱신해줄 함수
	
	virtual UC_SkillSystemComponent* GetSkillComponent() { return SkillSytemComponent; }
	
	void SetSkillList(UDA_SkillList* InSkillList) { SkillList = InSkillList; }
	UDA_SkillList* GetSkillList() { return SkillList; }

	UFUNCTION(Server, Reliable)
	void ServerSetEnemy(AC_SMSCharacter* Character);

	UFUNCTION(Client, Reliable)
	void ClientSetEnemy(AC_SMSCharacter* Character);

	UFUNCTION(Server, Reliable)
	void ServerTakeEffect(TSubclassOf<AC_SMSEffectINS> EffectClass);

	UFUNCTION(NetMulticast, Reliable)
	void MultiTakeEffect(TSubclassOf<AC_SMSEffectINS> EffectClass);

	UFUNCTION(Server, Reliable)
	void ServerSetCurSkill(AC_SMSSkillINS* _Skill);

	UFUNCTION(NetMulticast, Reliable)
	void MultiSetCurSkill(AC_SMSSkillINS* _Skill);

#pragma region Montage

public:
	UFUNCTION(Server, Reliable)
	void ServerPlayMontage(UAnimMontage* Montage, const FName& SectionName = "Default"); //서버몽타주

	UFUNCTION(NetMulticast, Reliable)
	void MultiPlayMontage(UAnimMontage* Montage, const FName& SectionName = "Default"); //멀티몽타주

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void ServerStopMontage();//서버 몽타주종료

	UFUNCTION(NetMulticast, Reliable)
	void MultiStopMontage();// 멀티 몽타주종료

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Montage)
	UAnimMontage* DeathMontage; // 사망 몽타주

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	UAnimMontage* ParryingMontage; // 사망 몽타주

	UFUNCTION(NetMulticast,Reliable)
	void MultiCharacterDamage(AActor*DamageCauser,float DamageAmount);

	UFUNCTION(Server, Reliable)
	void ServerSetAbsolute(bool _Absolute);
	
#pragma endregion
public:
	TMap<FString, AC_SMSEffectINS*> CurEffects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UC_SkillSystemComponent* SkillSytemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkillDataAsset")
	UDA_SkillList* SkillList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite )
	bool Absolute = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	EPlayerJob CharacterJob = EPlayerJob::NONE;

private:
	FTimerHandle InitSkillTimerHandle;

	void InitSkill();
};
