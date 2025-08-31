#pragma once

#include "CoreMinimal.h"
#include "Character/C_SMSCharacter.h"
#include "Component/C_SkillSystemComponent.h"
#include "../Manager/C_QuestManager.h"
#include "InputMappingContext.h"
#include "C_PlayerCharacter.generated.h"


class UC_PartySystemComponent;
class UC_ParticleSystemComponent;
class UC_InventoryComponent;
class UC_EquipmentComponent;

class AC_SMSQuest;
UCLASS()
class STRONGMETALSTONE_API AC_PlayerCharacter : public AC_SMSCharacter
{
	GENERATED_BODY()

private:
	const float CAPSULE_RADIUS = 42.0f; //캡슐 크기
	const float CAPSULE_HalfHEIGHT = 96.0f; // 캡슐 절반높이	
	const float TRACE_INTER_DISTANCE = 500.0f; //상호작용 거리
	const float TRACE_COMMUNITY_DISTANCE = 2000.0f; //커뮤창 거리
	const float TRACE_TARGET_DISTANCE = 2300.0f; //타게팅 거리
	const float TRACE_TARGET_RADIUS = 400.0f; //타겟 범위
	const float WALK_SPEED = 200.0f;
	const float RUN_SPEED = 600.0f;

protected:
	AC_PlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override; //리플리케이션
	virtual void Death(AC_SMSCharacter* Killer)override;

	UFUNCTION(Client,Reliable)
	void ClientDeath(AC_SMSCharacter* Killer, AC_SMSCharacter* _MyCharacter);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	//UFUNCTION(Client, Reliable)
	virtual void Update() override;	
	virtual void Init() override;

	UFUNCTION()
	void InitMinimap();

	UFUNCTION(Server, Reliable)
	void ServerInit();

	UFUNCTION(NetMulticast, Reliable)
	void MultiInit();

	UFUNCTION(Server, Reliable)
	void ServerUpdate(FCharacterInfo _CharacterInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MultiUpdate(FCharacterInfo _CharacterInfo);

	UFUNCTION(Client, Reliable)
	void ClientUpdate();

	UFUNCTION(Server, Reliable)
	void ServerPartyUpdate(FPartyData _PartyData);

	UFUNCTION(NetMulticast, Reliable)
	void MultiPartyUpdate(FPartyInfo _PartyInfo);

	UFUNCTION(Server, Reliable)
	void ServerSetTarget(AC_SMSCharacter* Target);

	UFUNCTION(Server, Reliable)
	void ServerUpdateHP(int32 ChangePoint);

	UFUNCTION(NetMulticast, Reliable)
	void MultiUpdateHP(int32 ChangePoint);
	

	void CheckLocalControl();

	void IncreaseExp(unsigned int Exp);


	UC_InventoryComponent*   GetInventoryComponent() { return InventoryComponent; };
	UC_EquipmentComponent*   GetEquipmentComponent() { return EquipmentComponent; }

	void SetCollisionActive();

#pragma region InputAction

public:
	//---------------------------------------기본 공통 움직임---------------------------------------
	virtual void Move(FVector2D Value); //움직임

	void Look(FVector2D Value); //시야 변경

	void InterActor(float Value); //상호작용

	void OpenCommunity(float Value); // 커뮤창 열기

	UFUNCTION(Server,Reliable)
	void ServerRun();
	UFUNCTION(NetMulticast,Reliable)
	void MultiRun();

	UFUNCTION(Server,Reliable)
	void ServerWalk();

	//--------------------------------------------- UI ---------------------------------------------
	void ToggleInventory(float Value); // 인벤 생성/제거 함수

	void OnOffMouse(float Value); // 마우스 생성,제거 Alt 함수

	//-------------------------------------------- 포션 --------------------------------------------
	
	virtual void ConsumeOne(float Value);	// 스킬5
	virtual void ConsumeTwo(float Value);	// 스킬6

#pragma endregion

	UFUNCTION()
	virtual void OnRep_CurHeat();

	virtual void SetColor();
	virtual void NewColor(float Value); // 커스터마이징 색 갱신(슬라이더)
	virtual void ButtonColor(FLinearColor Color,int32 Index);

	UFUNCTION(Server,Reliable)
	void ServerOnShield();

protected:
	void FindTarget();

	UFUNCTION(Server, Reliable)
	void ServerLoadQuest();

	UFUNCTION(Client, Reliable)
	void ClientAddQuest(FQuestData NewQuestData);
#pragma region Components

public:
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CustomBoom; //커스터마이징 카메라 스프링암

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CustomCamera; //커스터마이징 카메라

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom; //기본 카메라 스프링암

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera; //기본 카메라

protected:
	// 미니맵 카메라 붐 (SpringArm)
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* MiniMapCameraBoom;

	// 미니맵용 SceneCaptureComponent2D
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USceneCaptureComponent2D* SceneCaptureComponent;

	// 미니맵에 표시될 플레이어 아이콘
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly, Category = "MiniMap")
	class UPaperSpriteComponent* MiniMapIcon;

	// 미니맵이 그려질 Sprite
	UPROPERTY()
	class UPaperSprite* MiniMapSprite;

	// 미니맵 Sprite에 적용될 Material
	UPROPERTY()
	class UMaterialInstance* MiniMapMaterial;

public:
	// 파티 시스템
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UC_PartySystemComponent* PartySytemComponent;

	// 파티클 시스템
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UC_ParticleSystemComponent* ParticleSystemComponent;

	// 인벤토리 시스템
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UC_InventoryComponent* InventoryComponent;

	// 장비 시스템
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UC_EquipmentComponent* EquipmentComponent;

#pragma endregion

public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FCharacterInfo CharacterInfo; // 캐릭터 정보

	UPROPERTY()
	FPartyInfo PartyInfo; //파티정보

	AC_PlayerCharacter* SelectPlayerCharacter;

	UPROPERTY(Replicated)
	AC_SMSCharacter* TargetCharacter;

	UPROPERTY(Replicated,BlueprintReadWrite,VisibleAnywhere)
	TArray<AC_SMSQuest*> QuestArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Montage)
	class UAnimMontage* RideMontage; //스킬 몽타주

	class UAnimMontage* TitleMontage; //스킬 몽타주

	bool bOnMouse = false;
	bool bOnParrying = false; // 패링 변수
	bool bOnShield = false; // 쉴드상태 변수	

	FTimerHandle BattleInitTH; // 배틀 시간초기화 관련 TH

private:
	FTimerHandle InitCheckTimerHandle;
	FTimerHandle PartyTimerHandle;

	// NPC 혹은 Item 저장
	AActor* OverlappedActor;




	// 
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PreviousViewTarget;


	// 클라이언트에서 시점 전환 요청
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_SetViewTarget(AActor* NewViewTarget, float BlendTime = 3.0f);

	// 클라이언트에서 시점 복귀 요청
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void Client_ResetViewTarget(float BlendTime = 3.0f);


	// 클라이언트에서 카메라 쉐이크
	UFUNCTION(Client, Reliable)
	void PlayCameraShake2(TSubclassOf<UCameraShakeBase> ShakeClass);

	// 입력 맵핑 제거 / 추가
	UFUNCTION(Client, Reliable, BlueprintCallable)
	void RemoveMappingContext();

	UFUNCTION(Client, Reliable, BlueprintCallable)
	void AddMappingContext();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Look;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Mode")
	bool bIsTopDownMode = false;



	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void OnInvincibility();

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void OffInvincibility();

	UFUNCTION(Client,Reliable)
	void ClientSetLoading(bool _bOnLoading, bool _OnDungeon);

};