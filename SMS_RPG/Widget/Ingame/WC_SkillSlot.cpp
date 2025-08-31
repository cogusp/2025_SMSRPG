#include "Widget/Ingame/WC_SkillSlot.h"
#include "Character/C_PlayerCharacter.h"
#include "Component/C_SkillSystemComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UWC_SkillSlot::UWC_SkillSlot()
{
	// Material �ҷ�����
	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Script/Engine.Material'/Game/Material/M_Cooldown.M_Cooldown'"));

	if (MatFinder.Succeeded())
	{
		// ��Ÿ�� UI�� ���� Material Interface
		CooldownMat = MatFinder.Object;
	}
}

void UWC_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayerPawn())
	{
		// ������Ʈ ����
		if (AC_PlayerCharacter* Player = Cast<AC_PlayerCharacter>(GetOwningPlayerPawn()))
		{
			SkillComponent = Player->GetSkillComponent();
		}
	}
}

void UWC_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// ��Ÿ�� ������Ʈ
	if (bOnCooldown)
	{
		// ���� �ð�
		Elapsed += InDeltaTime;
		UpdateCooldown();
	}
}

FReply UWC_SkillSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return UWC_Slot::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UWC_SkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	UWC_Slot::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UWC_SkillSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return UWC_Slot::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

bool UWC_SkillSlot::DropSlot(UWC_SkillSlot* OtherSlot)
{
	// �巡���� ������ ������� ��� ��ȯ
	if (!OtherSlot || !OtherSlot->SkillData.SkillData || !SkillComponent) return false;

	// ��ųâ���� �巡���� ���
	if (SkillInput == ESkillInput::NONE) return false;

	if (SkillData.SkillInstance->OnCool == false) return false;

	if (OtherSlot->SkillData.SkillInstance)
	{
		if (OtherSlot->SkillData.SkillInstance->OnCool == false) return false;
	}

	ExchangeSlot(OtherSlot);

	return true;
}

void UWC_SkillSlot::SetSlot(FSkillData InSkillData)
{
	// ��ų ������ ����
	SkillData = InSkillData;

	if (SkillData.SkillData)
	{
		// ������ �̹��� ����
		ItemImage = SkillData.SkillData->Icon;
		// ��Ÿ�� ����
		Duration = SkillData.SkillData->CoolTime;
	}

	// ������Ʈ
	RefreshSlot();
}

void UWC_SkillSlot::ExchangeSlot(UWC_SkillSlot* OtherSlot)
{
	FSkillData TempSkillData = SkillData;

	// �����Կ��� ���������� �̵��� ���
	if (OtherSlot->SkillInput != ESkillInput::NONE)
	{
		SkillComponent->ServerRegisterSkillSlot(SkillInput, OtherSlot->SkillData);
		SkillComponent->ServerRegisterSkillSlot(OtherSlot->SkillInput, TempSkillData);
		OtherSlot->RefreshSlot();
	}
	else
	{
		if (SkillComponent->CheckSkillSlot(OtherSlot->SkillData))
		{
			SkillComponent->ServerRegisterSkillSlot(SkillInput, OtherSlot->SkillData);
		}
	}

	RefreshSlot();
}

void UWC_SkillSlot::ClearSlot()
{
	// ���� ���� �����
	SkillData = FSkillData();
	// ��ų �̹��� ����
	ItemImage = nullptr;

	RefreshSlot();
}

void UWC_SkillSlot::RefreshSlot()
{
	FSlateBrush EmptyBrush;
	// �ν��Ͻ� ����
	M_Skill = UMaterialInstanceDynamic::Create(CooldownMat, this);

	// ������ �����Ͱ� ���� ���
	if (SkillData.SkillData)
	{
		EmptyBrush.TintColor = FSlateColor(FLinearColor::White);
		ItemSlot->SetBrush(EmptyBrush);
		M_Skill->SetTextureParameterValue("BaseTexture", ItemImage);
		ItemSlot->SetBrushFromMaterial(M_Skill);
	}
	// ������ �����Ͱ� ���� ���
	else
	{
		// �귯�ø� �ʱ�ȭ�ϰ� �����ϰ� ����
		EmptyBrush.TintColor = FSlateColor(FLinearColor::Transparent);
		ItemSlot->SetBrush(EmptyBrush);
		M_Skill->SetTextureParameterValue("BaseTexture", nullptr);
	}
}

void UWC_SkillSlot::TriggerCooldown()
{
	// Material ���� Percent �ʱ�ȭ
	M_Skill->SetScalarParameterValue("CooldownPercent", 0.f);
}

void UWC_SkillSlot::StartCooldown()
{
	// Ÿ�̸� ����
	bOnCooldown = true;
}

void UWC_SkillSlot::UpdateCooldown()
{
	// Percent ���
	float Percent = Elapsed / Duration;

	// Material ���� CooldownPercent�� ����
	M_Skill->SetScalarParameterValue("CooldownPercent", FMath::Clamp(Percent, 0.f, 1.f));

	if (Percent >= 1.f)
	{		
		InitCooldown();
	}
}

void UWC_SkillSlot::InitCooldown()
{
	if (!SkillData.SkillData) return;

	// �ʱ�ȭ
	Elapsed = 0.f;
	Duration = SkillData.SkillData->CoolTime;

	// ��Ÿ�� ���� ����
	bOnCooldown = false;

	// Material ���� Percent �ʱ�ȭ
	M_Skill->SetScalarParameterValue("CooldownPercent", 1.f);
}