#include "PlayerController/C_WorldPlayerController.h"
#include "InputActionValue.h"
#include "HUD/C_WorldHUD.h"
#include "C_SMSGameInstance.h"
#include "Character/C_PlayerCharacter.h"
#include "GameMode/C_WorldGameMode.h"
#include "Actor/Item/C_Item.h"
#include "Quest/C_SMSQuest.h"
#include "Character/Riding/C_SMSRiding.h"
#include "Character/C_Warrior.h"
#include "Character/C_Mage.h"
#include "GameFramework/CharacterMovementComponent.h"


void AC_WorldPlayerController::BeginPlay()
{
	Super::BeginPlay(); 

	if (IsLocalController())
	{
		if (CastWCharacter(GetPawn()))
		{
			if (Cast<AC_Warrior>(GetPawn()))
			{
				SoundManager->PlayBGM(SoundManager->BGM_JULU); // ���ϴ� BGM ����
			}
			else if (Cast<AC_Mage>(GetPawn()))
			{
				SoundManager->PlayBGM(SoundManager->BGM_Maple); // ���ϴ� BGM ����
			}
		}
	
	}
	

	
}

//�������� ȣ���
void AC_WorldPlayerController::PawnLeavingGame()
{
	AC_PlayerCharacter* LeavingCharacter;
	
	//���� ������ ĳ����, ���� �������� ���� ����
	if (CastWCharacter(GetPawn()))
	{
		LeavingCharacter = CastWCharacter(GetPawn());
	}
	else
	{
		LeavingCharacter = RidingOwner;
	}
	UpdateCharacterInfoCSV(LeavingCharacter->CharacterInfo);
	CastWGM(GetMyGM())->LogOut(this, LeavingCharacter);
	
	Super::PawnLeavingGame();
}

void AC_WorldPlayerController::ServerLogOutCharacter_Implementation(APlayerController* PC, AC_PlayerCharacter* _Character)
{
	CastWGM(GetMyGM())->LogOut(PC, _Character);
}

void AC_WorldPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	//�⺻ IMC�� �߰�
	Subsystem->AddMappingContext(MoveIMC, 0);
	Subsystem->AddMappingContext(LookIMC, 0);
	Subsystem->AddMappingContext(NoneGameIMC, 0);
	Subsystem->AddMappingContext(AttackIMC, 0);

	// Enhanced Input�� ���� ����
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::Attack);
		EnhancedInput->BindAction(SkillOneAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::SkillOne);
		EnhancedInput->BindAction(SkillOneAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::SkillOneGoing);
		EnhancedInput->BindAction(SkillOneAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::SkillOneCompleted);
		EnhancedInput->BindAction(SkillTwoAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::SkillTwo);
		EnhancedInput->BindAction(SkillTwoAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::SkillTwoGoing);
		EnhancedInput->BindAction(SkillTwoAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::SkillTwoCompleted);
		EnhancedInput->BindAction(SkillThreeAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::SkillThree);
		EnhancedInput->BindAction(SkillThreeAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::SkillThreeGoing);
		EnhancedInput->BindAction(SkillThreeAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::SkillThreeCompleted);
		EnhancedInput->BindAction(SkillFourAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::SkillFour);
		EnhancedInput->BindAction(SkillFourAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::SkillFourGoing);
		EnhancedInput->BindAction(SkillFourAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::SkillFourCompleted);
		EnhancedInput->BindAction(SkillQAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::SkillQ);
		EnhancedInput->BindAction(SkillQAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::SkillQGoing);
		EnhancedInput->BindAction(SkillQAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::SkillQCompleted);
		EnhancedInput->BindAction(SkillEAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::SkillE);
		EnhancedInput->BindAction(SkillEAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::SkillEGoing);
		EnhancedInput->BindAction(SkillEAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::SkillECompleted);
		EnhancedInput->BindAction(HoldAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::Hold);
		EnhancedInput->BindAction(HoldAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::HoldGoing);
		EnhancedInput->BindAction(HoldAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::HoldCompleted);
	
		EnhancedInput->BindAction(ItemOneAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::ConsumeOne);
		EnhancedInput->BindAction(ItemTwoAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::ConsumeTwo);

		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::Move);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::Look);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::Jump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::JumpCancel);
		EnhancedInput->BindAction(MouseAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::OnOffMouse);
		EnhancedInput->BindAction(DashAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::Dash);
		EnhancedInput->BindAction(DashAction, ETriggerEvent::Completed, this, &AC_WorldPlayerController::DashEnd);
		EnhancedInput->BindAction(MouseRightAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::OpenCommunity);
		EnhancedInput->BindAction(EscapeAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::Escape);
		EnhancedInput->BindAction(InterAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::InterActor);
		EnhancedInput->BindAction(RideAction, ETriggerEvent::Triggered, this, &AC_WorldPlayerController::Ride);
		
		// ä�� Ȱ��ȭ
		EnhancedInput->BindAction(ChatAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::ChatButtonPressed);
		// �κ��丮 Ȱ��ȭ
		EnhancedInput->BindAction(InvenAction, ETriggerEvent::Started, this, &AC_WorldPlayerController::ToggleInventory);
	}
}

void AC_WorldPlayerController::SeamlessTravelTo(APlayerController* NewPC)
{
	Super::SeamlessTravelTo(NewPC);
	NewPC->Possess(GetPawn());
}

void AC_WorldPlayerController::ClientSpawnCharacter_Implementation(APlayerController* PC)
{	
	ServerSpawnCharacter(CastWGI(GetMyGI())->CharacterInfo,PC);
}

void AC_WorldPlayerController::ServerSpawnCharacter_Implementation(FCharacterInfo _CharacterInfo, APlayerController* PC)
{
	CastWGM(GetMyGM())->SpawnPlayer(_CharacterInfo,PC);
}
void AC_WorldPlayerController::RemoveIMC(UInputMappingContext* IMC)
{
	if (Subsystem)
	{
		Subsystem->RemoveMappingContext(IMC); // �Ű����� IMC ����
	}
}
void AC_WorldPlayerController::ServerUpdateCharacter_Implementation(FCharacterInfo _NewCharacterInfo)
{
	UpdateCharacterInfoCSV(_NewCharacterInfo);
}
void AC_WorldPlayerController::Move(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	AC_SMSRiding* Horse = Cast<AC_SMSRiding>(GetPawn());
	if (MyCharacter)
	{
		// �̵� �� ó��
		MyCharacter->Move(Value.Get<FVector2D>());
	}
	else if(Horse)
	{
		Horse->Move(Value.Get<FVector2D>());
	}
}

void AC_WorldPlayerController::Dash(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	AC_SMSRiding* Horse = Cast<AC_SMSRiding>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Dash); // �뽬 ����
	}
	else if (Horse)
	{
		Horse->ServerRun();
		Horse->Run();
	}
}

void AC_WorldPlayerController::DashEnd(const FInputActionValue& Value)
{
	AC_SMSRiding* Horse = Cast<AC_SMSRiding>(GetPawn());
	if (Horse)
	{
		Horse->ServerRun();
		Horse->Run();
	}
}

void AC_WorldPlayerController::Attack(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		// ����
		MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Attack);

		if (Cast<AC_Warrior>(MyCharacter))
		{
			Cast<AC_Warrior>(MyCharacter)->ServerDelayOverHeat();
		}

	}

}

void AC_WorldPlayerController::Look(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	AC_SMSRiding* Horse = Cast<AC_SMSRiding>(GetPawn());
	if (MyCharacter)
	{
		// ���콺 �þ� �� ó��
		MyCharacter->Look(Value.Get<FVector2D>()* MouseSpeed);
	}
	else if (Horse)
	{
		Horse->LookAt(Value.Get<FVector2D>()* MouseSpeed);
	}
}

void AC_WorldPlayerController::InterActor(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		// ��ȣ�ۿ�
		MyCharacter->InterActor(Value.Get<float>());
	}
}

void AC_WorldPlayerController::OpenCommunity(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		// Ŀ��â ����
		MyCharacter->OpenCommunity(Value.Get<float>());
	}
}

void AC_WorldPlayerController::Jump(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	AC_SMSRiding* Horse = Cast<AC_SMSRiding>(GetPawn());
	if (MyCharacter)
	{
		// ���� ó��
		MyCharacter->Jump();
	}
	else if (Horse)
	{
		Horse->Jump();
	}
}

void AC_WorldPlayerController::JumpCancel(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		// ���� ����
		MyCharacter->StopJumping();
	}
}

void AC_WorldPlayerController::Ride(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	ServerRide();
}

void AC_WorldPlayerController::ServerRide_Implementation()
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		if (!HorseINS&&!bOnRiding)
		{
			// �����ϰ� �ű⿡ ž��
			MyCharacter->ServerPlayMontage(MyCharacter->RideMontage);
			RidingOwner = MyCharacter;
			HorseINS = GetWorld()->SpawnActor<AC_SMSRiding>(HorseClass, MyCharacter->GetActorLocation(), MyCharacter->GetActorRotation());
			Possess(HorseINS);
			HorseINS->OnMount(RidingOwner);
		}
	}
	else if (HorseINS)
	{
		Possess(RidingOwner);
		RidingOwner->ServerStopMontage();
		HorseINS->OnMount(RidingOwner);
		HorseINS = nullptr;
	}
}


void AC_WorldPlayerController::ToggleInventory(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		// �κ� ����/���� �Լ�
		MyCharacter->ToggleInventory(Value.Get<float>());
	}
}

void AC_WorldPlayerController::ChatButtonPressed(const FInputActionValue& Value)
{
	AC_WorldHUD* WorldHUD = CastWHUD((GetMyHUD()));

	if (WorldHUD)
	{
		// ä��â Ȱ��ȭ
		WorldHUD->SetChatWidgetVisible(true);
		// ä�� �Է� Ȱ��ȭ
		WorldHUD->ActivateChat();
	}
}

void AC_WorldPlayerController::OnOffMouse(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		// ���콺 ����
		MyCharacter->OnOffMouse(Value.Get<float>());
	}
}

void AC_WorldPlayerController::SkillOne(const FInputActionValue& Value)
{
	if (bOnAttacking)return;

	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	
	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Skill1);
		MyCharacter->SkillSytemComponent->StartSkills(ESkillInput::SKILL1);

		if (Cast<AC_Warrior>(MyCharacter))
		{
			Cast<AC_Warrior>(MyCharacter)->ServerDelayOverHeat();
		}

	}
}

void AC_WorldPlayerController::SkillOneGoing(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());  

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->HoldSkill(MyCharacter->SkillSytemComponent->Skill1);  // ��ų1 ���
		MyCharacter->SkillSytemComponent->HoldSkills(ESkillInput::SKILL1);
	}
}

void AC_WorldPlayerController::SkillOneCompleted(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->EndSkill(MyCharacter->SkillSytemComponent->Skill1);  // ��ų1 �Ϸ�
		MyCharacter->SkillSytemComponent->EndSkills(ESkillInput::SKILL1);
	}
}

void AC_WorldPlayerController::SkillTwo(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Skill2);
		MyCharacter->SkillSytemComponent->StartSkills(ESkillInput::SKILL2);

		if (Cast<AC_Warrior>(MyCharacter))
		{
			Cast<AC_Warrior>(MyCharacter)->ServerDelayOverHeat();
		}

	}
}

void AC_WorldPlayerController::SkillTwoGoing(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->HoldSkill(MyCharacter->SkillSytemComponent->Skill2);  // ��ų2 ���
		MyCharacter->SkillSytemComponent->HoldSkills(ESkillInput::SKILL2);
	}
}

void AC_WorldPlayerController::SkillTwoCompleted(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->EndSkill(MyCharacter->SkillSytemComponent->Skill2);  // ��ų2 �Ϸ�
		MyCharacter->SkillSytemComponent->EndSkills(ESkillInput::SKILL2);
	}
}

void AC_WorldPlayerController::SkillThree(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Skill3);
		MyCharacter->SkillSytemComponent->StartSkills(ESkillInput::SKILL3);

		if (Cast<AC_Warrior>(MyCharacter))
		{
			Cast<AC_Warrior>(MyCharacter)->ServerDelayOverHeat();
		}

	}
}

void AC_WorldPlayerController::SkillThreeGoing(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->HoldSkill(MyCharacter->SkillSytemComponent->Skill3);  // ��ų3 ���
		MyCharacter->SkillSytemComponent->HoldSkills(ESkillInput::SKILL3);
	}
}

void AC_WorldPlayerController::SkillThreeCompleted(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->EndSkill(MyCharacter->SkillSytemComponent->Skill3);  // ��ų3 �Ϸ�
		MyCharacter->SkillSytemComponent->EndSkills(ESkillInput::SKILL3);
	}
}

void AC_WorldPlayerController::SkillFour(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Skill4);
		MyCharacter->SkillSytemComponent->StartSkills(ESkillInput::SKILL4);

		if (Cast<AC_Warrior>(MyCharacter))
		{
			Cast<AC_Warrior>(MyCharacter)->ServerDelayOverHeat();
		}

	}
}

void AC_WorldPlayerController::SkillFourGoing(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->HoldSkill(MyCharacter->SkillSytemComponent->Skill4);  // ��ų4 ���
		MyCharacter->SkillSytemComponent->HoldSkills(ESkillInput::SKILL4);
	}
}

void AC_WorldPlayerController::SkillFourCompleted(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		//MyCharacter->SkillSytemComponent->EndSkill(MyCharacter->SkillSytemComponent->Skill4);  // ��ų4 �Ϸ�
		MyCharacter->SkillSytemComponent->EndSkills(ESkillInput::SKILL4);
	}
}

void AC_WorldPlayerController::SkillQ(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->StartSkills(ESkillInput::SKILLQ);

		if (Cast<AC_Warrior>(MyCharacter))
		{
			Cast<AC_Warrior>(MyCharacter)->ServerDelayOverHeat();
		}

	}
}

void AC_WorldPlayerController::SkillQGoing(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());


	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->HoldSkills(ESkillInput::SKILLQ);
	}

}

void AC_WorldPlayerController::SkillQCompleted(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->EndSkills(ESkillInput::SKILLQ);
	}
}

void AC_WorldPlayerController::SkillE(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->StartSkills(ESkillInput::SKILLE);

		if (Cast<AC_Warrior>(MyCharacter))
		{
			Cast<AC_Warrior>(MyCharacter)->ServerDelayOverHeat();
		}

	}
}

void AC_WorldPlayerController::SkillEGoing(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->HoldSkills(ESkillInput::SKILLE);  // ��ųE ���
	}
}

void AC_WorldPlayerController::SkillECompleted(const FInputActionValue& Value)
{
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->EndSkills(ESkillInput::SKILLE);  // ��ųE �Ϸ�
	}
}

void AC_WorldPlayerController::ConsumeOne(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		// �����ۻ��1
		MyCharacter->ConsumeOne(Value.Get<float>());
	}
}

void AC_WorldPlayerController::ConsumeTwo(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		// �����ۻ��2
		MyCharacter->ConsumeTwo(Value.Get<float>());
	}
}

void AC_WorldPlayerController::Hold(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Hold);
	}
}

void AC_WorldPlayerController::HoldGoing(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->HoldSkill(MyCharacter->SkillSytemComponent->Hold);  // Hold ���
	}
}

void AC_WorldPlayerController::HoldCompleted(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());

	if (MyCharacter)
	{
		MyCharacter->SkillSytemComponent->EndSkill(MyCharacter->SkillSytemComponent->Hold);  // Hold �Ϸ�
	}
}

void AC_WorldPlayerController::Escape(const FInputActionValue& Value)
{
	if (bOnAttacking)return;
	AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn());
	if (MyCharacter)
	{
		// Ż���
		MyCharacter->SkillSytemComponent->PlaySkill(MyCharacter->SkillSytemComponent->Escape);
	}
}

void AC_WorldPlayerController::ServerDestroyItem_Implementation(AC_Item* Item)
{
	if (Item)
	{
		Item->SetActorEnableCollision(false);
		Item->Destroy();

		if (AC_PlayerCharacter* MyCharacter = Cast<AC_PlayerCharacter>(GetPawn()))
		{
			MyCharacter->SetCollisionActive();
		}
	}
}

// ���Ӹ�忡 ���� �� �޽��� ����
void AC_WorldPlayerController::ServerSendChat_Implementation(const FString& Message)
{
	AC_WorldGameMode* WorldGM = CastWGM(GetMyGM());

	if (WorldGM)
	{
		WorldGM->SendChatMessage(Message);
	}
}

// ��� �÷��̾�� ä���� ǥ��
void AC_WorldPlayerController::ClientSendChat_Implementation(const FString& Message)
{
	AC_WorldHUD* WorldHUD = CastWHUD((GetMyHUD()));

	if (WorldHUD)
	{
		WorldHUD->AddChatMessage(Message);
	}
}

void AC_WorldPlayerController::ServerAddQuestToCSV_Implementation(FQuestData _QuestInfo)
{
	AddQuestToCSV(_QuestInfo);
}


void AC_WorldPlayerController::ServerDeleteQuestCSV_Implementation(FQuestData _QuestInfo)
{
	DeleteQuestCSV(_QuestInfo);
}
