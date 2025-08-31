#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_Slot.h"
#include "WC_ItemSlot.generated.h"

class UC_InventoryComponent;
class UC_EquipmentComponent;
class UWC_InventoryWidget;
class UTextBlock;

UCLASS()
class STRONGMETALSTONE_API UWC_ItemSlot : public UWC_Slot
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	// ���콺 ��ư ���� ��
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// ���콺 Ŭ�� �Ǵ� ��Ŭ�� ó��
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// Drag ����
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	// Drop ����
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	

	// ������ ��� �� ����� �Լ�
	UFUNCTION(BlueprintCallable)
	bool DropSlot(UWC_ItemSlot* OtherSlot);

	// ���� Ŭ�� �� ����� �Լ�
	UFUNCTION()
	void SlotLeftClicked();
	
	// ���� Ŭ�� �� ����� �Լ�
	UFUNCTION()
	void SlotRightClicked();

	// ������ �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintCallable)
	void SetSlot(FInventorySlot InSlotData);

	// ��ġ��
	UFUNCTION(BlueprintCallable)
	bool MergeSlot(UWC_ItemSlot* OtherSlot);

	// ������ �̵�
	UFUNCTION(BlueprintCallable)
	void MoveData(UWC_ItemSlot* OtherSlot);


	// ������ ���� Ÿ�� �Ǻ�
	static bool IsItemCompatibleWithSlot(ESlotType InSlotType, EItemType InItemType, EItemSubType InItemSubType = EItemSubType::NONE);
	
	// ���� �ε��� ��ȯ �Լ�
	int32 GetEquipmentSlotIndex(ESlotType InSlotType);


	// ���� ����� �Լ�
	virtual void ClearSlot() override;

	// ���� ���� �Լ�
	virtual void RefreshSlot() override;


	// ������ ���� ��ȯ �Լ�
	UDA_ItemData* GetItemData() const { return SlotData.ItemData; }
	int32 GetItemCount() const { return SlotData.ItemQuantity; }

	// ������ ���� ���� �Լ�
	void SetParentWidget(UWC_InventoryWidget* InWidget) { ParentWidget = InWidget; }

public:
	// ������ ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInventorySlot SlotData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotType SlotType = ESlotType::NONE;

	UPROPERTY()
	int32 SlotIndex = -1;

	UPROPERTY()
	UWC_InventoryWidget* ParentWidget;

protected:
	// ���� �ؽ�Ʈ
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;


	// ������Ʈ
	UPROPERTY()
	UC_InventoryComponent* InventoryComponent;

	UPROPERTY()
	UC_EquipmentComponent* EquipmentComponent;


	// �巡�� ���� ����
	bool bDragDetected = false; 
};
