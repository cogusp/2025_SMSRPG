#include "Widget/Ingame/WC_MerchantWidget.h"
#include "Widget/Ingame/WC_ScriptWidget.h"

void UWC_MerchantWidget::NativeConstruct()
{
	ScriptWidget->SetTextName(TEXT("데카르트"));
	ScriptWidget->SetTextScript(TEXT("어서 오세요!"));
}