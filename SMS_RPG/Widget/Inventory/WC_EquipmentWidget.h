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

	// 장비 데이터 표시용
	UFUNCTION()
	void RefreshEquipment();

public:
	UFUNCTION()
	void OnCloseButtonClicked();

public:
	// 슬롯 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UWC_ItemSlot*> SlotArray;

	// 슬롯 위젯 클래스 지정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWC_ItemSlot> SlotWidgetClass;

	// 직업별 이미지
	UPROPERTY(meta = (BindWidget))
	class UImage* JobImage;

	// 각 Slot들
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

	// 나가기 버튼
	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;

protected:
	UC_EquipmentComponent* EquipmentComponent;
};