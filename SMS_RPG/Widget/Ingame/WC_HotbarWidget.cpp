#include "Widget/Ingame/WC_HotbarWidget.h"
#include "Widget/Ingame/WC_SkillSlot.h"
#include "Widget/Inventory/WC_ItemSlot.h"
#include "Widget/Inventory/C_ItemDragDropOperation.h"

#include "Character/C_PlayerCharacter.h"
#include "Character/Skill/C_SMSSkill.h"

#include "Component/C_EquipmentComponent.h"
#include "Component/C_SkillSystemComponent.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"

void UWC_HotbarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Component�� ��� Delegate ���ε�
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn()))
		{
			EquipmentComponent = Player->GetEquipmentComponent();
			SkillComponent = Player->GetSkillComponent();
			if (!SkillComponent || !EquipmentComponent) return;
			EquipmentComponent->OnEquipmentUpdate.AddDynamic(this, &UWC_HotbarWidget::RefreshHotbar);
			SkillComponent->OnSkillSlotUpdate.AddDynamic(this, &UWC_HotbarWidget::RefreshHotbar);
			SkillComponent->OnSkillRegister.AddDynamic(this, &UWC_HotbarWidget::SetCooldownBind);
		}
	}
	
	// ���� �ʱ�ȭ
	InitSlot();
	// UI ����


	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
}

bool UWC_HotbarWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UWC_HotbarWidget::InitSlot()
{
	// ���� ���� Ŭ������ ���� �ڽ��� ����� ��� ��ȯ
	if (!SkillSlotClass || !ItemSlotClass || !Skillbar || !Itembar) return;
	
	// �ڽ� ���빰 �����
	Skillbar->ClearChildren();
	Itembar->ClearChildren();

	if (!SkillComponent) return;

	// ��ų ����
	for (int32 i = 0; i < 6; ++i)
	{
		// ���� ���� ����
		UWC_SkillSlot* SkillSlot = CreateWidget<UWC_SkillSlot>(this, SkillSlotClass);
		SkillSlot->SetVisibility(ESlateVisibility::Visible);

		// SKILL1���� SKILL4���� ����
		ESkillInput SkillSlotType = static_cast<ESkillInput>(static_cast<uint8>(ESkillInput::SKILL1) + i);

		if(i==0)
		SkillSlot->Text_SlotNum->SetText(FText::FromString("1"));
		else if (i == 1)
		{
			SkillSlot->Text_SlotNum->SetText(FText::FromString("2"));
		}
		else if (i == 2)
		{
			SkillSlot->Text_SlotNum->SetText(FText::FromString("3"));

		}
		else if (i == 3)
		{
			SkillSlot->Text_SlotNum->SetText(FText::FromString("4"));
		}
		else if (i == 4)
		{
			SkillSlot->Text_SlotNum->SetText(FText::FromString("Q"));
		}
		else
		{
			SkillSlot->Text_SlotNum->SetText(FText::FromString("E"));
		}

		// ��ĭ���� ����
		FSkillData EmptySlot;
		SkillSlot->SetSlot(EmptySlot);
		// ���� Ÿ�� ����
		SkillSlot->SkillInput = SkillSlotType;

		// HorizontalBox�� �ڽ����� ���� �߰�
		Skillbar->AddChildToHorizontalBox(SkillSlot);
	}

	for (int32 i = 0; i <= 1; ++i)
	{
		// ���� ���� ����
		UWC_ItemSlot* ItemSlot = CreateWidget<UWC_ItemSlot>(this, ItemSlotClass);
		ItemSlot->SetVisibility(ESlateVisibility::Visible);

		// ��ĭ���� ����
		FInventorySlot EmptySlot;
		ItemSlot->SetSlot(EmptySlot);

		// ���� Ÿ�� ����
		ESlotType SkillSlotEnum = static_cast<ESlotType>(static_cast<uint8>(ESlotType::Potion1) + i);
		ItemSlot->SlotType = SkillSlotEnum;

		// HorizontalBox�� �ڽ����� ���� �߰�
		Itembar->AddChildToHorizontalBox(ItemSlot);
	}

	SetCooldownBind();
	RefreshHotbar();
}

void UWC_HotbarWidget::RefreshHotbar()
{
	// ������Ʈ�� ����ִ� ��� ��ȯ
	if (!EquipmentComponent || !SkillComponent) return;

	for (int i = 0; i < 6; ++i)
	{
		UWC_SkillSlot* SkillSlot = Cast<UWC_SkillSlot>(Skillbar->GetChildAt(i));

		// ESkillSlot�� ������� ��� ���� �迭
		ESkillInput SkillSlotType = static_cast<ESkillInput>(static_cast<uint8>(ESkillInput::SKILL1) + i);

		// SkillComponent���� �ش� ���� Ÿ���� ��ų�� ������
		if (FSkillData* SkillData = SkillComponent->SlotSkills.Find(SkillSlotType))
		{
			SkillSlot->SetSlot(*SkillData);
			SkillSlot->SkillInput = SkillSlotType;
		}
		// ���ٸ�
		else
		{
			FSkillData EmptySlot;
			SkillSlot->SetSlot(EmptySlot);
			SkillSlot->SkillInput = ESkillInput::NONE;
		}
	}

	TArray<FInventorySlot>& EquipmentSlots = EquipmentComponent->GetEquipmentData();
	
	for (int32 i = 5; i <= 6; ++i)
	{
		UWC_ItemSlot* ItemSlot = Cast<UWC_ItemSlot>(Itembar->GetChildAt(i - 5));

		// �����ϰ� �ִ� �������� ���ٸ�
		if (EquipmentSlots[i].IsEmpty())
		{
			FInventorySlot EmptySlot;
			ItemSlot->SetSlot(EmptySlot);
		}
		// �ִٸ� ���� ����
		else
		{
			ItemSlot->SetSlot(EquipmentSlots[i]);
		}
	}

	SetCooldownBind();
}

void UWC_HotbarWidget::SetCooldownBind()
{
	for (int i = 0; i < 6; ++i)
	{
		UWC_SkillSlot* SkillSlot = Cast<UWC_SkillSlot>(Skillbar->GetChildAt(i));

		if (!SkillSlot->SkillData.SkillInstance) return;

		if (!SkillSlot->SkillData.SkillInstance->OnTriggerCooldown.IsAlreadyBound(this, &UWC_HotbarWidget::TriggerCooldown))
		{
			// ��Ÿ�� �Լ� ���ε�
			SkillSlot->SkillData.SkillInstance->OnTriggerCooldown.AddDynamic(this, &UWC_HotbarWidget::TriggerCooldown);
			SkillSlot->SkillData.SkillInstance->OnStartCooldown.AddDynamic(this, &UWC_HotbarWidget::StartCooldown);
		}
	}
}

void UWC_HotbarWidget::TriggerCooldown(AC_SMSSkillINS* InSkillInstance)
{
	// ������Ʈ�� ����ִ� ��� ��ȯ
	if (!EquipmentComponent || !SkillComponent) return;

	for (int i = 0; i < 6; ++i)
	{
		UWC_SkillSlot* SkillSlot = Cast<UWC_SkillSlot>(Skillbar->GetChildAt(i));

		if (SkillSlot->SkillData.SkillInstance->SkillAsset->SkillName != InSkillInstance->SkillAsset->SkillName) continue;

		SkillSlot->TriggerCooldown();
	}
}

void UWC_HotbarWidget::StartCooldown(AC_SMSSkillINS* InSkillInstance)
{
	// ������Ʈ�� ����ִ� ��� ��ȯ
	if (!EquipmentComponent || !SkillComponent) return;

	for (int i = 0; i < 6; ++i)
	{
		UWC_SkillSlot* SkillSlot = Cast<UWC_SkillSlot>(Skillbar->GetChildAt(i));

		if (SkillSlot->SkillData.SkillInstance->SkillAsset->SkillName != InSkillInstance->SkillAsset->SkillName) continue;

		SkillSlot->StartCooldown();
	}
}