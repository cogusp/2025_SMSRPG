#include "Widget/Ingame/WC_SkillSlot.h"
#include "Character/C_PlayerCharacter.h"
#include "Component/C_SkillSystemComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UWC_SkillSlot::UWC_SkillSlot()
{
	// Material 불러오기
	static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Script/Engine.Material'/Game/Material/M_Cooldown.M_Cooldown'"));

	if (MatFinder.Succeeded())
	{
		// 쿨타임 UI를 위한 Material Interface
		CooldownMat = MatFinder.Object;
	}
}

void UWC_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (GetOwningPlayerPawn())
	{
		// 컴포넌트 저장
		if (AC_PlayerCharacter* Player = Cast<AC_PlayerCharacter>(GetOwningPlayerPawn()))
		{
			SkillComponent = Player->GetSkillComponent();
		}
	}
}

void UWC_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 쿨타임 업데이트
	if (bOnCooldown)
	{
		// 현재 시간
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
	// 드래그한 슬롯이 비어있을 경우 반환
	if (!OtherSlot || !OtherSlot->SkillData.SkillData || !SkillComponent) return false;

	// 스킬창으로 드래그한 경우
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
	// 스킬 데이터 설정
	SkillData = InSkillData;

	if (SkillData.SkillData)
	{
		// 아이템 이미지 설정
		ItemImage = SkillData.SkillData->Icon;
		// 쿨타임 설정
		Duration = SkillData.SkillData->CoolTime;
	}

	// 업데이트
	RefreshSlot();
}

void UWC_SkillSlot::ExchangeSlot(UWC_SkillSlot* OtherSlot)
{
	FSkillData TempSkillData = SkillData;

	// 퀵슬롯에서 퀵슬롯으로 이동한 경우
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
	// 기존 슬롯 지우기
	SkillData = FSkillData();
	// 스킬 이미지 설정
	ItemImage = nullptr;

	RefreshSlot();
}

void UWC_SkillSlot::RefreshSlot()
{
	FSlateBrush EmptyBrush;
	// 인스턴스 생성
	M_Skill = UMaterialInstanceDynamic::Create(CooldownMat, this);

	// 아이템 데이터가 있을 경우
	if (SkillData.SkillData)
	{
		EmptyBrush.TintColor = FSlateColor(FLinearColor::White);
		ItemSlot->SetBrush(EmptyBrush);
		M_Skill->SetTextureParameterValue("BaseTexture", ItemImage);
		ItemSlot->SetBrushFromMaterial(M_Skill);
	}
	// 아이템 데이터가 없을 경우
	else
	{
		// 브러시를 초기화하고 투명하게 설정
		EmptyBrush.TintColor = FSlateColor(FLinearColor::Transparent);
		ItemSlot->SetBrush(EmptyBrush);
		M_Skill->SetTextureParameterValue("BaseTexture", nullptr);
	}
}

void UWC_SkillSlot::TriggerCooldown()
{
	// Material 내의 Percent 초기화
	M_Skill->SetScalarParameterValue("CooldownPercent", 0.f);
}

void UWC_SkillSlot::StartCooldown()
{
	// 타이머 시작
	bOnCooldown = true;
}

void UWC_SkillSlot::UpdateCooldown()
{
	// Percent 계산
	float Percent = Elapsed / Duration;

	// Material 내의 CooldownPercent에 적용
	M_Skill->SetScalarParameterValue("CooldownPercent", FMath::Clamp(Percent, 0.f, 1.f));

	if (Percent >= 1.f)
	{		
		InitCooldown();
	}
}

void UWC_SkillSlot::InitCooldown()
{
	if (!SkillData.SkillData) return;

	// 초기화
	Elapsed = 0.f;
	Duration = SkillData.SkillData->CoolTime;

	// 쿨타임 상태 변경
	bOnCooldown = false;

	// Material 내의 Percent 초기화
	M_Skill->SetScalarParameterValue("CooldownPercent", 1.f);
}