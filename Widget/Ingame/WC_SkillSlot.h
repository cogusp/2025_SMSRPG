#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_Slot.h"
#include "WC_SkillSlot.generated.h"

class UC_SMSSkill;
class UC_SkillSystemComponent;

UCLASS()
class STRONGMETALSTONE_API UWC_SkillSlot : public UWC_Slot
{
	GENERATED_BODY()
private:
	bool isInit = false;

public:
	UWC_SkillSlot();

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// 마우스 버튼 누를 때
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// Drag 감지
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	// Drop 감지
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// Drop 했을 때 실행될 함수
	bool DropSlot(UWC_SkillSlot* OtherSlot);

	// 아이템 초기화 함수
	UFUNCTION(BlueprintCallable)
	void SetSlot(FSkillData InSkillData);

	// 이동
	UFUNCTION(BlueprintCallable)
	void ExchangeSlot(UWC_SkillSlot* OtherSlot);

	// 슬롯 지우는 함수
	virtual void ClearSlot() override;

	// 슬롯 갱신 함수
	virtual void RefreshSlot() override;

#pragma region Cooldown

	// 스킬이 사용됐을 때, 어둡게 만들어줄 함수
	UFUNCTION()
	void TriggerCooldown();

	// 쿨타임이 실행될 때 실행될 함수
	UFUNCTION()
	void StartCooldown();

	// 쿨타임 업데이트 함수
	UFUNCTION()
	void UpdateCooldown();

	UFUNCTION()
	void InitCooldown();

#pragma endregion

public:
	UPROPERTY(BlueprintReadOnly)
	FSkillData SkillData; 

	UPROPERTY(BlueprintReadOnly)
	ESkillInput SkillInput = ESkillInput::NONE;
	
#pragma region Cooldown

	// 쿨타임 UI Material
	UPROPERTY()
	UMaterialInterface* CooldownMat;

	// 각 스킬에 쿨타임 UI를 적용할 Material Instance
	UPROPERTY()
	UMaterialInstanceDynamic* M_Skill;

	// 쿨타임 Tick 변수
	bool bOnCooldown = false;

	// 현재 쿨타임 시간
	float Elapsed = 0.f;

	// 쿨타임 지속 시간
	float Duration = 0.f;

#pragma endregion

protected:
	UPROPERTY()
	UC_SkillSystemComponent* SkillComponent;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SlotNum;
};
