#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_SellAreaWidget.generated.h"

class UC_ItemDragDropOperation;
class UWC_SellItemSlot;

UCLASS()
class STRONGMETALSTONE_API UWC_SellAreaWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct();

public:
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	// 나가기 버튼 누르면 실행
	UFUNCTION()
	void OnCloseButtonClicked();

	void InitSellItemGrid(class UDA_ItemDataList* ItemDataList);

public:
	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;	// 나가기 버튼

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* SellScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UWC_SellItemSlot> SellItemSlotClass;
};