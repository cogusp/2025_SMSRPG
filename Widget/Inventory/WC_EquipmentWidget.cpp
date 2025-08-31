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

	// ���� ���� Ŭ������ ����� ��� ��ȯ
	if (!SlotWidgetClass) return;

	// InventoryComponent�� ��� Delegate ���ε�
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

	// â �ݱ� ��ư �ʱ�ȭ
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// â �ݱ� ��ư�� �Լ� ���ε�
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

	// ���� �迭�� ���Ե� ����
	SlotArray[0] = HelmetSlot;
	SlotArray[1] = UpperArmorSlot;
	SlotArray[2] = LowerArmorSlot;
	SlotArray[3] = ShoesSlot;
	SlotArray[4] = WeaponSlot;

	// ���� �迭 ���� ���� Ÿ�� ����
	for (int i = 0; i < 5; ++i)
	{
		ESlotType InSlotType = static_cast<ESlotType>(static_cast<uint8>(ESlotType::HELMET) + i);
		SlotArray[i]->SlotType = InSlotType;
	}
}

void UWC_EquipmentWidget::RefreshEquipment()
{
	// ��� ������Ʈ�� ����ִ� ��� ��ȯ
	if (!EquipmentComponent) return;

	// ��� ������Ʈ�� �����͸� �ҷ��ͼ� ����
	const TArray<FInventorySlot>& EquipmentSlots = EquipmentComponent->GetEquipmentData();

	for (int32 i = 0; i < SlotArray.Num(); ++i)
	{
		// ������ �����ͺ��� ���� ���� ���
		if (i < EquipmentSlots.Num())
		{
			// ������ �����ͷ� ����
			const FInventorySlot& SlotData = EquipmentSlots[i];

			SlotArray[i]->SetSlot(SlotData);
		}
		// ������ �����ͺ��� ���� ū ���
		else
		{
			// ������ ���
			SlotArray[i]->ClearSlot();
		}
	}
}

void UWC_EquipmentWidget::OnCloseButtonClicked()
{
	// �� ����â �ݱ�
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetEquipmentWidgetVisible(false);
	}
}