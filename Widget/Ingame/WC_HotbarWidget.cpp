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

	// Component를 얻어 Delegate 바인딩
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
	
	// 슬롯 초기화
	InitSlot();
	// UI 갱신


	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
}

bool UWC_HotbarWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}

void UWC_HotbarWidget::InitSlot()
{
	// 슬롯 위젯 클래스와 수평 박스가 비었을 경우 반환
	if (!SkillSlotClass || !ItemSlotClass || !Skillbar || !Itembar) return;
	
	// 박스 내용물 지우기
	Skillbar->ClearChildren();
	Itembar->ClearChildren();

	if (!SkillComponent) return;

	// 스킬 슬롯
	for (int32 i = 0; i < 6; ++i)
	{
		// 슬롯 위젯 생성
		UWC_SkillSlot* SkillSlot = CreateWidget<UWC_SkillSlot>(this, SkillSlotClass);
		SkillSlot->SetVisibility(ESlateVisibility::Visible);

		// SKILL1부터 SKILL4까지 적용
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

		// 빈칸으로 설정
		FSkillData EmptySlot;
		SkillSlot->SetSlot(EmptySlot);
		// 슬롯 타입 지정
		SkillSlot->SkillInput = SkillSlotType;

		// HorizontalBox에 자식으로 슬롯 추가
		Skillbar->AddChildToHorizontalBox(SkillSlot);
	}

	for (int32 i = 0; i <= 1; ++i)
	{
		// 슬롯 위젯 생성
		UWC_ItemSlot* ItemSlot = CreateWidget<UWC_ItemSlot>(this, ItemSlotClass);
		ItemSlot->SetVisibility(ESlateVisibility::Visible);

		// 빈칸으로 설정
		FInventorySlot EmptySlot;
		ItemSlot->SetSlot(EmptySlot);

		// 슬롯 타입 지정
		ESlotType SkillSlotEnum = static_cast<ESlotType>(static_cast<uint8>(ESlotType::Potion1) + i);
		ItemSlot->SlotType = SkillSlotEnum;

		// HorizontalBox에 자식으로 슬롯 추가
		Itembar->AddChildToHorizontalBox(ItemSlot);
	}

	SetCooldownBind();
	RefreshHotbar();
}

void UWC_HotbarWidget::RefreshHotbar()
{
	// 컴포넌트가 비어있는 경우 반환
	if (!EquipmentComponent || !SkillComponent) return;

	for (int i = 0; i < 6; ++i)
	{
		UWC_SkillSlot* SkillSlot = Cast<UWC_SkillSlot>(Skillbar->GetChildAt(i));

		// ESkillSlot을 순서대로 얻기 위한 배열
		ESkillInput SkillSlotType = static_cast<ESkillInput>(static_cast<uint8>(ESkillInput::SKILL1) + i);

		// SkillComponent에서 해당 슬롯 타입의 스킬을 가져옴
		if (FSkillData* SkillData = SkillComponent->SlotSkills.Find(SkillSlotType))
		{
			SkillSlot->SetSlot(*SkillData);
			SkillSlot->SkillInput = SkillSlotType;
		}
		// 없다면
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

		// 장착하고 있는 아이템이 없다면
		if (EquipmentSlots[i].IsEmpty())
		{
			FInventorySlot EmptySlot;
			ItemSlot->SetSlot(EmptySlot);
		}
		// 있다면 슬롯 설정
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
			// 쿨타임 함수 바인딩
			SkillSlot->SkillData.SkillInstance->OnTriggerCooldown.AddDynamic(this, &UWC_HotbarWidget::TriggerCooldown);
			SkillSlot->SkillData.SkillInstance->OnStartCooldown.AddDynamic(this, &UWC_HotbarWidget::StartCooldown);
		}
	}
}

void UWC_HotbarWidget::TriggerCooldown(AC_SMSSkillINS* InSkillInstance)
{
	// 컴포넌트가 비어있는 경우 반환
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
	// 컴포넌트가 비어있는 경우 반환
	if (!EquipmentComponent || !SkillComponent) return;

	for (int i = 0; i < 6; ++i)
	{
		UWC_SkillSlot* SkillSlot = Cast<UWC_SkillSlot>(Skillbar->GetChildAt(i));

		if (SkillSlot->SkillData.SkillInstance->SkillAsset->SkillName != InSkillInstance->SkillAsset->SkillName) continue;

		SkillSlot->StartCooldown();
	}
}