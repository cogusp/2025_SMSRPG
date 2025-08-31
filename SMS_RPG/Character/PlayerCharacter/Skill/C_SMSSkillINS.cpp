#include "Character/Skill/C_SMSSkillINS.h"
#include "Character/C_PlayerCharacter.h"

#include "HUD/C_WorldHUD.h"
#include "PlayerController/C_WorldPlayerController.h"

AC_SMSSkillINS::AC_SMSSkillINS()
{
	bReplicates = true;
}

void AC_SMSSkillINS::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

void AC_SMSSkillINS::Init()
{
	CoolTime = SkillAsset->CoolTime;
	DemandedMana = SkillAsset->DemandedMana;
}

void AC_SMSSkillINS::StartOnce(AC_SMSCharacter* Character)
{
	if (OnCool)
	{
		if (Character == nullptr)
			Character = CastWCharacter(GetMyCharacter());

		AC_PlayerCharacter* Player = CastWCharacter(GetMyCharacter());
		AC_WorldPlayerController* WPC = Cast< AC_WorldPlayerController>(Player->GetController());


		if (CastWCharacter(Character))
		{
			if (Player->CharacterInfo.Curmp >= DemandedMana)
			{
				OnTriggerCooldown.Broadcast(this);
				OnStartCooldown.Broadcast(this);

				OnCool = false;
				DelayCoolTime();

				WPC->Subsystem->RemoveMappingContext(WPC->AttackIMC);
				Character->ServerPlayMontage(SkillAsset->SkillMontage);
				Player->CharacterInfo.Curmp -= DemandedMana;
				Player->Update();
			}
		}
		else
		{
			Character->ServerPlayMontage(SkillAsset->SkillMontage);
		}
		
	}
}

void AC_SMSSkillINS::Holding(AC_SMSCharacter* Character)
{
	if (Character == nullptr)
		Character = CastWCharacter(GetMyCharacter());

	//홀드중일때 로직
}

void AC_SMSSkillINS::EndHold(AC_SMSCharacter* Character)
{
	OnStartCooldown.Broadcast(this);

	if (Character == nullptr)
		Character = CastWCharacter(GetMyCharacter());

	Character->ServerStopMontage();
}

void AC_SMSSkillINS::DelayCoolTime()
{
	GetWorld()->GetTimerManager().SetTimer(CoolTimerHandle, this, &AC_SMSSkillINS::CoolTimeInit, CoolTime, false);
}

void AC_SMSSkillINS::CoolTimeInit()
{
	OnCool = true;
	GetWorld()->GetTimerManager().ClearTimer(CoolTimerHandle);
}

void AC_SMSSkillINS::SkillComboReset()
{
	SkillCombo = 0;
	GetWorld()->GetTimerManager().ClearTimer(ComboTimerHandle);
}

void AC_SMSSkillINS::AttackIMCReset()
{
	AC_PlayerCharacter* Player = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WPC = Cast< AC_WorldPlayerController>(Player->GetController());
	WPC->Subsystem->AddMappingContext(WPC->AttackIMC, 0);
	GetWorld()->GetTimerManager().ClearTimer(AttackIMCTimerHandle);
}
