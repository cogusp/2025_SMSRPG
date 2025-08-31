#include "Widget/Ingame/WC_ItemNameWidget.h"
#include "Components/TextBlock.h"

void UWC_ItemNameWidget::SetTextName(const FText& text)
{
	TEXT_Name->SetText(text);
}