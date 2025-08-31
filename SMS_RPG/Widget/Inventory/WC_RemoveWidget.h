#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_RemoveWidget.generated.h"

class UWC_SMSButton;
class UDA_ItemData;
class UC_InventoryComponent;

UCLASS()
class STRONGMETALSTONE_API UWC_RemoveWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTextChangedCallback(const FText& Text);

public:
	UFUNCTION()
	void OnYesButtonClicked();

	UFUNCTION()
	void OnNoButtonClicked();

	UFUNCTION()
	void SetItemData(UDA_ItemData* InItemData);

public:
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_Yes;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_No;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TEXT_ItemCount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_ItemName;

private:
	UDA_ItemData* ItemData;

	UC_InventoryComponent* InventoryComponent;

	FString PrevStr = "";
};
