#include "Widget/Inventory/WC_EquipmentWidget.h"
#include "Widget/WC_SMSButton.h"
#include "Widget/Inventory/WC_ItemSlot.h"

#include "Character/C_PlayerCharacter.h"
#include "HUD/C_WorldHUD.h"
#include "Components/Image.h"
#include "Data/DA_ItemData.h"
#include "Component/C_EquipmentComponent.h"

void UWC_EquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 슬롯 위젯 클래스가 비었을 경우 반환
	if (!SlotWidgetClass) return;

	// InventoryComponent를 얻어 Delegate 바인딩
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn()))
		{
			EquipmentComponent = Player->GetEquipmentComponent();

			if (EquipmentComponent)
			{
				EquipmentComponent->OnEquipmentUpdate.AddDynamic(this, &UWC_EquipmentWidget::RefreshEquipment);
			}
		}
	}

	// 창 닫기 버튼 초기화
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// 창 닫기 버튼에 함수 바인딩
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_EquipmentWidget::OnCloseButtonClicked);

	Init();
	RefreshEquipment();
}

void UWC_EquipmentWidget::Init()
{
	FCharacterInfo CharacterInfo = CastWCharacter(GetMyCharacter())->CharacterInfo;
	CharacterInfo.Job = EPlayerJob::WARRIOR;
	if (JobImage)
	{
		switch (CharacterInfo.Job)
		{
		case EPlayerJob::NONE:
			JobImage->SetBrushFromTexture(nullptr);
			break;
		case EPlayerJob::WARRIOR:
			JobImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/01_Character/Warrior")));
			break;
		case EPlayerJob::MAGE:
			JobImage->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/01_Character/Mage")));
			break;
		default:
			JobImage->SetBrushFromTexture(nullptr);
			break;
		}
	}

	// 슬롯 배열에 슬롯들 배정
	SlotArray[0] = HelmetSlot;
	SlotArray[1] = UpperArmorSlot;
	SlotArray[2] = LowerArmorSlot;
	SlotArray[3] = ShoesSlot;
	SlotArray[4] = WeaponSlot;

	// 슬롯 배열 내의 슬롯 타입 지정
	for (int i = 0; i < 5; ++i)
	{
		ESlotType InSlotType = static_cast<ESlotType>(static_cast<uint8>(ESlotType::HELMET) + i);
		SlotArray[i]->SlotType = InSlotType;
	}
}

void UWC_EquipmentWidget::RefreshEquipment()
{
	// 장비 컴포넌트가 비어있는 경우 반환
	if (!EquipmentComponent) return;

	// 장비 컴포넌트의 데이터를 불러와서 저장
	const TArray<FInventorySlot>& EquipmentSlots = EquipmentComponent->GetEquipmentData();

	for (int32 i = 0; i < SlotArray.Num(); ++i)
	{
		// 가져온 데이터보다 수가 작은 경우
		if (i < EquipmentSlots.Num())
		{
			// 가져온 데이터로 설정
			const FInventorySlot& SlotData = EquipmentSlots[i];

			SlotArray[i]->SetSlot(SlotData);
		}
		// 가져온 데이터보다 수가 큰 경우
		else
		{
			// 슬롯을 비움
			SlotArray[i]->ClearSlot();
		}
	}
}

void UWC_EquipmentWidget::OnCloseButtonClicked()
{
	// 내 정보창 닫기
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetEquipmentWidgetVisible(false);
	}
}