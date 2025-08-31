#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_SellItemSlot.generated.h"

class UC_InventoryComponent;
class UTextBlock;

UCLASS()
class STRONGMETALSTONE_API UWC_SellItemSlot : public UWC_SMSUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION()
	void OnSlotClicked();

	// ������ �ʱ�ȭ �Լ�
	UFUNCTION(BlueprintCallable)
	void SetSlot(UDA_ItemData* NewItemData);

	// ������ ���� ��ȯ �Լ�
	UDA_ItemData* GetItemData() const { return ItemData; }

	UFUNCTION(BlueprintCallable)
	virtual void RefreshSlot();

public:
	// ������ ������
	UPROPERTY(BlueprintReadOnly)
	UDA_ItemData* ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UTexture2D* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemPrice;

	UPROPERTY()
	UC_InventoryComponent* InventoryComponent;

protected:
	// ���� �̹���
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemSlot;
};
