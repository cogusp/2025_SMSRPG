#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_EquipmentWidget.generated.h"

class UWC_ItemSlot;
class UC_EquipmentComponent;

UCLASS()
class STRONGMETALSTONE_API UWC_EquipmentWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	void Init();

	// ��� ������ ǥ�ÿ�
	UFUNCTION()
	void RefreshEquipment();

public:
	UFUNCTION()
	void OnCloseButtonClicked();

public:
	// ���� �迭
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UWC_ItemSlot*> SlotArray;

	// ���� ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWC_ItemSlot> SlotWidgetClass;

	// ������ �̹���
	UPROPERTY(meta = (BindWidget))
	class UImage* JobImage;

	// �� Slot��
	UPROPERTY(meta = (BindWidget))
	UWC_ItemSlot* HelmetSlot;

	UPROPERTY(meta = (BindWidget))
	UWC_ItemSlot* UpperArmorSlot;

	UPROPERTY(meta = (BindWidget))
	UWC_ItemSlot* LowerArmorSlot;

	UPROPERTY(meta = (BindWidget))
	UWC_ItemSlot* ShoesSlot;

	UPROPERTY(meta = (BindWidget))
	UWC_ItemSlot* WeaponSlot;

	// ������ ��ư
	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;

protected:
	UC_EquipmentComponent* EquipmentComponent;
};