#pragma once

#include "CoreMinimal.h"
#include "PlayerController/C_SMSPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "C_WorldPlayerController.generated.h"

class AC_Item;
class AC_PlayerCharacter;

//스킬은 반드시 HOLD로 만들 것
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
	void RemoveIMC(UInputMappingContext* IMC); //IMC 제거

#pragma region MovementFunction
	//---------------------------------------기본 공통 움직임---------------------------------------
	void Move(const FInputActionValue& Value); //움직임

	void Dash(const FInputActionValue& Value); //대쉬
	void DashEnd(const FInputActionValue& Value); //대쉬 종료

	void Attack(const FInputActionValue& Value); //공격

	void Look(const FInputActionValue& Value); //시야 변경

	void InterActor(const FInputActionValue& Value); //상호작용

	void OpenCommunity(const FInputActionValue& Value); // 커뮤창 열기

	void Jump(const FInputActionValue& Value); // 점프

	void JumpCancel(const FInputActionValue& Value); // 점프 끝

	virtual void Ride(const FInputActionValue& Value); // 탑승, 하차

	//--------------------------------------------- UI ---------------------------------------------
	void ToggleInventory(const FInputActionValue& Value); // 인벤 생성/제거 함수

	void ChatButtonPressed(const FInputActionValue& Value);	

	void OnOffMouse(const FInputActionValue& Value); // 마우스 생성,제거 Alt 함수

	//-------------------------------------------- 스킬 --------------------------------------------
	void SkillOne(const FInputActionValue& Value);		// 스킬1
	void SkillOneGoing(const FInputActionValue& Value);
	void SkillOneCompleted(const FInputActionValue& Value);
	void SkillTwo(const FInputActionValue& Value);		// 스킬2
	void SkillTwoGoing(const FInputActionValue& Value);
	void SkillTwoCompleted(const FInputActionValue& Value);
	void SkillThree(const FInputActionValue& Value);	// 스킬3
	void SkillThreeGoing(const FInputActionValue& Value);
	void SkillThreeCompleted(const FInputActionValue& Value);
	void SkillFour(const FInputActionValue& Value);		// 스킬4
	void SkillFourGoing(const FInputActionValue& Value);
	void SkillFourCompleted(const FInputActionValue& Value);
	void SkillQ(const FInputActionValue& Value);		// 스킬Q
	void SkillQGoing(const FInputActionValue& Value);
	void SkillQCompleted(const FInputActionValue& Value);
	void SkillE(const FInputActionValue& Value);		// 스킬E
	void SkillEGoing(const FInputActionValue& Value);
	void SkillECompleted(const FInputActionValue& Value);

	void ConsumeOne(const FInputActionValue& Value);	// 소모템1
	void ConsumeTwo(const FInputActionValue& Value);	// 소모템2

	void Hold(const FInputActionValue& Value);		  // F스킬사용
	void HoldGoing(const FInputActionValue& Value);		// F스킬 계속
	void HoldCompleted(const FInputActionValue& Value);  // F스킬 중단

	void Escape(const FInputActionValue& Value);		// 탈출기
	
#pragma endregion

public:
	// Server 메시지 전송
	UFUNCTION(Server, Reliable)
	void ServerSendChat(const FString& Message);

	// Client 메시지 전송
	UFUNCTION(Client, Reliable)
	void ClientSendChat(const FString& Message);

	// Server 아이템 삭제
	UFUNCTION(Server, Reliable)
	void ServerDestroyItem(AC_Item* Item);

	UFUNCTION(Server,Reliable)
	void ServerRide();

#pragma region InputAction

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;		//상하좌우 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;		//마우스 시점 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;		//점프 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;		//대쉬 IA

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;	//공격 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* InterAction;	//상호작용 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* MouseAction;	//마우스 Alt 관련 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* InvenAction;	//인벤토리 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChatAction;		// 채팅 입력 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillOneAction; // 스킬1 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillTwoAction; //스킬2 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* SkillThreeAction; //스킬3 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* SkillFourAction; //스킬4 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* SkillQAction; //스킬Q IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* SkillEAction; //스킬E IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* ItemOneAction; //아이템사용1 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* ItemTwoAction; //아이템사용2 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* HoldAction; //G스킬 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* MouseRightAction; //마우스우클릭 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* EscapeAction; //탈출기 IA

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowprivateAccess = "true"))
	class UInputAction* RideAction; //탑승,하차 IA

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
	UInputMappingContext* MoveIMC; // 상하좌우 움직임 IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* LookIMC; // 마우스 시야 IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* NoneGameIMC; // 게임 외 IMC

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* AttackIMC; // 공격, 스킬 IMC

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
