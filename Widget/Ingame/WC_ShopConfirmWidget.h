#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_ShopConfirmWidget.generated.h"

class UTextBlock;
class UWC_SMSButton;
class UDA_ItemData;

UCLASS()
class STRONGMETALSTONE_API UWC_ShopConfirmWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetWidgetFromData(UDA_ItemData* InItemData, EShopMode InShopMode);

	UFUNCTION()
	void OnYesButtonClicked();

	UFUNCTION()
	void OnNoButtonClicked();

public:
	UPROPERTY(meta = (BindWidget))
	class UImage* ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemPrice;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_Yes;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_No;

private:
	UDA_ItemData* ItemData;

	EShopMode ShopMode;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ButtonSound;
};