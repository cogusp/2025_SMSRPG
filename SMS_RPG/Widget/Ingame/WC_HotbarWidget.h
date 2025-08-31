#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_HotbarWidget.generated.h"

class UC_EquipmentComponent;
class UC_SkillSystemComponent;
class UWC_SkillSlot;
class UWC_ItemSlot;
class UHorizontalBox;
class UC_ItemDragDropOperation;

UCLASS()
class STRONGMETALSTONE_API UWC_HotbarWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct();
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

public:
	UFUNCTION()
	void RefreshHotbar();

	UFUNCTION()
	void SetCooldownBind();
	
	UFUNCTION()
	void TriggerCooldown(AC_SMSSkillINS* InSkillInstance);

	UFUNCTION()
	void StartCooldown(AC_SMSSkillINS* InSkillInstance);

protected:
	UC_EquipmentComponent* EquipmentComponent;
	UC_SkillSystemComponent* SkillComponent;

public:
	void InitSlot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UWC_SkillSlot> SkillSlotClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UWC_ItemSlot> ItemSlotClass;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Skillbar;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Itembar;
};
