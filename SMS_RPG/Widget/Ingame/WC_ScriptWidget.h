#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_ScriptWidget.generated.h"

class UTextBlock;

UCLASS()
class STRONGMETALSTONE_API UWC_ScriptWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetTextName(const FString& InTextName);
	virtual void SetTextScript(const FString& InTextScript);

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Name;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Script;
};
