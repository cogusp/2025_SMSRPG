#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_InventoryWidget.generated.h"

class UWC_ItemSlot;
class UWC_ItemOptionWidget;
class UC_InventoryComponent;
class UCanvasPanel;

UCLASS()
class STRONGMETALSTONE_API UWC_InventoryWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// �κ��丮 ������ ǥ�ÿ�
	UFUNCTION()
	void RefreshInventory();

	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void SetShowOptionWidget(UWC_ItemSlot* InSlot, const FText& text);

public:
	// ���� �迭
	UPROPERTY()
	TArray<UWC_ItemSlot*> SlotArray;

	// ���� ���� Ŭ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWC_ItemSlot> SlotWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;	// ������ ��ư

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_Gold;

	// �ɼ� ����
	UPROPERTY()
	UWC_ItemOptionWidget* OptionWidget;

	// �ɼ� ���� Ŭ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWC_ItemOptionWidget> OptionWidgetClass;

protected:
	// ������ ���ε��� �׸���
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* InventoryGrid;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

	UPROPERTY(EditDefaultsOnly)
	int32 NumSlots = 30;

	UC_InventoryComponent* InventoryComponent;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BeginSound;
};
