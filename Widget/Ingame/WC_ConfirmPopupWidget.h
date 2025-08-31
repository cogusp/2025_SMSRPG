#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_ConfirmPopupWidget.generated.h"

class UWC_SMSButton;
class UDA_ItemData;
class UC_InventoryComponent;

UCLASS()
class STRONGMETALSTONE_API UWC_ConfirmPopupWidget : public UWC_SMSUserWidget
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
	void SetItemData(UDA_ItemData* InItemData, EShopMode InShopMode);

public:
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_Yes;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_No;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TEXT_ItemCount;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TEXT_TotalPrice;

private:
	UDA_ItemData* ItemData;

	EShopMode ShopMode;

	UC_InventoryComponent* InventoryComponent;

	FString PrevStr = "";

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* SellSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BuySound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* CancelSound;
};
