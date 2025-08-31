#include "Widget/Ingame/WC_ScriptWidget.h"
#include "Components/TextBlock.h"

void UWC_ScriptWidget::SetTextName(const FString& InTextName)
{
	TEXT_Name->SetText(FText::FromString(InTextName));
}

void UWC_ScriptWidget::SetTextScript(const FString& InTextScript)
{
	TEXT_Script->SetText(FText::FromString(InTextScript));
}