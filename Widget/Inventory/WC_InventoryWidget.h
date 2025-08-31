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

	// 인벤토리 데이터 표시용
	UFUNCTION()
	void RefreshInventory();

	UFUNCTION()
	void OnCloseButtonClicked();

	UFUNCTION()
	void SetShowOptionWidget(UWC_ItemSlot* InSlot, const FText& text);

public:
	// 슬롯 배열
	UPROPERTY()
	TArray<UWC_ItemSlot*> SlotArray;

	// 슬롯 위젯 클래스 지정
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWC_ItemSlot> SlotWidgetClass;

	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;	// 나가기 버튼

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_Gold;

	// 옵션 위젯
	UPROPERTY()
	UWC_ItemOptionWidget* OptionWidget;

	// 옵션 위젯 클래스 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UWC_ItemOptionWidget> OptionWidgetClass;

protected:
	// 위젯에 바인딩된 그리드
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
