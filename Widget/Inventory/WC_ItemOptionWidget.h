#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_ItemOptionWidget.generated.h"

class UWC_SMSButton;
class UWC_ItemSlot;
class UC_InventoryComponent;

UCLASS()
class STRONGMETALSTONE_API UWC_ItemOptionWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnUseButtonClicked();

	UFUNCTION()
	void OnRemoveButtonClicked();

public:
	UFUNCTION()
	void SetItemSlot(UWC_ItemSlot* InItemSlot) { ItemSlot = InItemSlot; }

	UFUNCTION()
	void SetButtonText(const FText& text);

private:
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_Use;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* BTN_Remove;


	// ÄÄÆ÷³ÍÆ®
	UPROPERTY()
	UC_InventoryComponent* InventoryComponent;

	// ÇöÀç ½½·Ô
	UPROPERTY()
	UWC_ItemSlot* ItemSlot;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BeginSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* UseSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* RemoveSound;
};