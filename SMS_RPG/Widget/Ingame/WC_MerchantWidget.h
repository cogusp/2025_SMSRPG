#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_MerchantWidget.generated.h"

class UWC_SMSButton;

UCLASS()
class STRONGMETALSTONE_API UWC_MerchantWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct();

public:
	UPROPERTY(meta = (BindWidget))
	class UWC_ScriptWidget* ScriptWidget;
};