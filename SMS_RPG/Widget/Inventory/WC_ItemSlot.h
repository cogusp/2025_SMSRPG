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
	// 마우스 버튼 누를 때
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// 마우스 클릭 또는 우클릭 처리
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// Drag 감지
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	// Drop 감지
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	

	// 아이템 드랍 시 실행될 함수
	UFUNCTION(BlueprintCallable)
	bool DropSlot(UWC_ItemSlot* OtherSlot);

	// 단일 클릭 시 실행될 함수
	UFUNCTION()
	void SlotLeftClicked();
	
	// 단일 클릭 시 실행될 함수
	UFUNCTION()
	void SlotRightClicked();

	// 아이템 초기화 함수
	UFUNCTION(BlueprintCallable)
	void SetSlot(FInventorySlot InSlotData);

	// 합치기
	UFUNCTION(BlueprintCallable)
	bool MergeSlot(UWC_ItemSlot* OtherSlot);

	// 데이터 이동
	UFUNCTION(BlueprintCallable)
	void MoveData(UWC_ItemSlot* OtherSlot);


	// 아이템 슬롯 타입 판별
	static bool IsItemCompatibleWithSlot(ESlotType InSlotType, EItemType InItemType, EItemSubType InItemSubType = EItemSubType::NONE);
	
	// 슬롯 인덱스 변환 함수
	int32 GetEquipmentSlotIndex(ESlotType InSlotType);


	// 슬롯 지우는 함수
	virtual void ClearSlot() override;

	// 슬롯 갱신 함수
	virtual void RefreshSlot() override;


	// 아이템 정보 반환 함수
	UDA_ItemData* GetItemData() const { return SlotData.ItemData; }
	int32 GetItemCount() const { return SlotData.ItemQuantity; }

	// 아이템 정보 설정 함수
	void SetParentWidget(UWC_InventoryWidget* InWidget) { ParentWidget = InWidget; }

public:
	// 아이템 데이터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInventorySlot SlotData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotType SlotType = ESlotType::NONE;

	UPROPERTY()
	int32 SlotIndex = -1;

	UPROPERTY()
	UWC_InventoryWidget* ParentWidget;

protected:
	// 수량 텍스트
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuantityText;


	// 컴포넌트
	UPROPERTY()
	UC_InventoryComponent* InventoryComponent;

	UPROPERTY()
	UC_EquipmentComponent* EquipmentComponent;


	// 드래그 감지 여부
	bool bDragDetected = false; 
};
