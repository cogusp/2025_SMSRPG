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

	// ���콺 ��ư ���� ��
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// Drag ����
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	// Drop ����
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// Drop ���� �� ����� �Լ�
	bool DropSlot(UWC_SkillSlot* OtherSlot);

	// ������ �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintCallable)
	void SetSlot(FSkillData InSkillData);

	// �̵�
	UFUNCTION(BlueprintCallable)
	void ExchangeSlot(UWC_SkillSlot* OtherSlot);

	// ���� ����� �Լ�
	virtual void ClearSlot() override;

	// ���� ���� �Լ�
	virtual void RefreshSlot() override;

#pragma region Cooldown

	// ��ų�� ������ ��, ��Ӱ� ������� �Լ�
	UFUNCTION()
	void TriggerCooldown();

	// ��Ÿ���� ����� �� ����� �Լ�
	UFUNCTION()
	void StartCooldown();

	// ��Ÿ�� ������Ʈ �Լ�
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

	// ��Ÿ�� UI Material
	UPROPERTY()
	UMaterialInterface* CooldownMat;

	// �� ��ų�� ��Ÿ�� UI�� ������ Material Instance
	UPROPERTY()
	UMaterialInstanceDynamic* M_Skill;

	// ��Ÿ�� Tick ����
	bool bOnCooldown = false;

	// ���� ��Ÿ�� �ð�
	float Elapsed = 0.f;

	// ��Ÿ�� ���� �ð�
	float Duration = 0.f;

#pragma endregion

protected:
	UPROPERTY()
	UC_SkillSystemComponent* SkillComponent;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SlotNum;
};
