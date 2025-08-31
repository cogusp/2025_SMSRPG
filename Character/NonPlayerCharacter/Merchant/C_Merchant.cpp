#include "Character/C_Merchant.h"
#include "HUD/C_WorldHUD.h"
#include "Data/DA_ItemDataList.h"

void AC_Merchant::Action(AC_PlayerCharacter* Character)
{
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetSellAreaWidgetVisible(true);
		WorldHUD->SetInventoryWidgetVisible(true);
		WorldHUD->SetMerchantWidgetVisible(true);
		WorldHUD->InitSellItemGrid(DA_ItemDataList);
	}
}