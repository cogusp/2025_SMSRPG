#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_ItemNameWidget.generated.h"

class UTextBlock;

UCLASS()
class STRONGMETALSTONE_API UWC_ItemNameWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
public:
	void SetTextName(const FText& text);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Name;
};
