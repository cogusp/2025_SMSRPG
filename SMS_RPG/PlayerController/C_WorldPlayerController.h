#pragma once

#include "CoreMinimal.h"
#include "PlayerController/C_SMSPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "C_WorldPlayerController.generated.h"

class AC_Item;
class AC_PlayerCharacter;

//��ų�� �ݵ�� HOLD�� ���� ��
UCLASS()
class STRONGMETALSTONE_API AC_WorldPlayerController : public AC_SMSPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Client, Reliable)
	void ClientSpawnCharacter(APlayerController* PC);

	UFUNCTION(Server, Reliable)
	void ServerSpawnCharacter(FCharacterInfo _CharacterInfo, APlayerController* PC);


	UFUNCTION(Server, Reliable)
	void ServerUpdateCharacter(FCharacterInfo _NewCharacterInfo);

	UFUNCTION(Server, Reliable)
	void ServerLogOutCharacter(APlayerController* PC, AC_PlayerCharacter* _Character);

	UFUNCTION(Server, Reliable)
	void ServerAddQuestToCSV(FQuestData _QuestInfo);

	UFUNCTION(Server, Reliable)
	void ServerDeleteQuestCSV(FQuestData _QuestInfo);
protected:
	virtual void BeginPlay() override;
	virtual void PawnLeavingGame() override;
	void SetupInputComponent() override;
	void SeamlessTravelTo(APlayerController* NewPC) override;

protected:
	void RemoveIMC(UInputMappingContext* IMC); //IMC ����

#pragma region MovementFunction
	//---------------------------------------�⺻ ���� ������---------------------------------------
	void Move(const FInputActionValue& Value); //������

	void Dash(const FInputActionValue& Value); //�뽬
	void DashEnd(const FInputActionValue& Value); //�뽬 ����

	void Attack(const FInputActionValue& Value); //����

	void Look(const FInputActionValue& Value); //�þ� ����

	void InterActor(const FInputActionValue& Value); //��ȣ�ۿ�

	void OpenCommunity(const FInputActionValue& Value); // Ŀ��â ����

	void Jump(const FInputActionValue& Value); // ����

	void JumpCancel(const FInputActionValue& Value); // ���� ��

	virtual void Ride(const FInputActionValue& Value); // ž��, ����

	//--------------------------------------------- UI ---------------------------------------------
	void ToggleInventory(const FInputActionValue& Value); // �κ� ����/���� �Լ�

	void ChatButtonPressed(const FInputActionValue& Value);	

	void OnOffMouse(const FInputActionValue& Value); // ���콺 ����,���� Alt �Լ�

	//-------------------------------------------- ��ų --------------------------------------------
	void SkillOne(const FInputActionValue& Value);		// ��ų1
	void SkillOneGoing(const FInputActionValue& Value);
	void SkillOneCompleted(const FInputActionValue& Value);
	void SkillTwo(const FInputActionValue& Value);		// ��ų2
	void SkillTwoGoing(const FInputActionValue& Value);
	void SkillTwoCompleted(const FInputActionValue& Value);
	void SkillThree(const FInputActionValue& Value);	// ��ų3
	void SkillThreeGoing(const FInputActionValue& Value);
	void SkillThreeCompleted(const FInputActionValue& Value);
	void SkillFour(const FInputActionValue& Value);		// ��ų4
	void SkillFourGoing(const FInputActionValue& Value);
	void SkillFourCompleted(const FInputActionValue& Value);
	void SkillQ(const FInputActionValue& Value);		// ��ųQ
	void SkillQGoing(const FInputActionValue& Value);
	void SkillQCompleted(const FInputActionValue& Value);
	void SkillE(const FInputActionValue& Value);		// ��ųE
	void SkillEGoing(const FInputActionValue& Value);
	void SkillECompleted(const FInputActionValue& Value);

	void ConsumeOne(const FInputActionValue& Value);	// �Ҹ���1
	void ConsumeTwo(const FInputActionValue& Value);	// �Ҹ���2

	void Hold(const FInputActionValue& Value);		  // F��ų���
	void HoldGoing(const FInputActionValue& Value);		// F��ų ���
	void HoldCompleted(const FInputActionValue& Value);  // F��ų �ߴ�

	void Escape(const FInputActionValue& Value);		// Ż���
	
#pragma endregion

public:
	// Server �޽��� ����
	UFUNCTION(Server, Reliable)
	void ServerSendChat(const FString& Message);

	// Client �޽��� ����
	UFUNCTION(Client, Reliable)
	void ClientSendChat(const FString& Message);

	// Server ������ ����
	UFUNCTION(Server, Reliable)
	void ServerDestroyItem(AC_Item* Item);

	UFUNCTION(Server,Reliable)
	void ServerRide();

#pragma region InputAction

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;		//�����¿� IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;		//���콺 ���� IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;		//���� IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;		//�뽬 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;	//���� IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* InterAction;	//��ȣ�ۿ� IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* MouseAction;	//���콺 Alt ���� IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* InvenAction;	//�κ��丮 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChatAction;		// ä�� �Է� IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillOneAction; // ��ų1 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillTwoAction; //��ų2 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillThreeAction; //��ų3 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* SkillFourAction; //��ų4 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* SkillQAction; //��ųQ IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* SkillEAction; //��ųE IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* ItemOneAction; //�����ۻ��1 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* ItemTwoAction; //�����ۻ��2 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* HoldAction; //G��ų IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* MouseRightAction; //���콺��Ŭ�� IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* EscapeAction; //Ż��� IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* RideAction; //ž��,���� IA

#pragma endregion

private:
	bool bSkillOneCharge = false;
	float SkillOneChargeStartTime = 0.0f;
	bool bSkillTwoCharge = false;
	float SkillTwoChargeStartTime = 0.0f;
	bool bSkillThreeCharge = false;
	float SkillThreeChargeStartTime = 0.0f;
	bool bSkillFourCharge = false;
	float SkillFourChargeStartTime = 0.0f;

#pragma region InputMappingContext

public:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MoveIMC; // �����¿� ������ IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* LookIMC; // ���콺 �þ� IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* NoneGameIMC; // ���� �� IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AttackIMC; // ����, ��ų IMC

	float MouseSpeed = 1.0f;

#pragma endregion
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor>HorseClass;

	UPROPERTY()
	class AC_SMSRiding* HorseINS;

	AC_PlayerCharacter* RidingOwner;

	bool bOnAttacking = false;
	bool bOnRiding = false;

	FTimerHandle RidingTH;
	




};
