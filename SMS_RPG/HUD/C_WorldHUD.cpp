#include "HUD/C_WorldHUD.h"

#include "Widget/Ingame/WC_PlayerWidget.h"
#include "Widget/Ingame/WC_CommunityWidget.h"
#include "Widget/Ingame/WC_MyInfoWidget.h"
#include "Widget/Ingame/WC_HotbarWidget.h"

#include "Widget/Ingame/WC_QuestWidget.h"
#include "Widget/Ingame/WC_QuestingWidget.h"
#include "Widget/Ingame/WC_QuestingTextWidget.h"

#include "Widget/Ingame/WC_DeathWidget.h"
#include "Widget/Ingame/WC_ResultWidget.h"
#include "Widget/Ingame/WC_ResultCharacterWidget.h"

#include "Widget/Ingame/WC_MerchantWidget.h"
#include "Widget/Ingame/WC_SellAreaWidget.h"
#include "Widget/Ingame/WC_ShopConfirmWidget.h"
#include "Widget/Ingame/WC_ConfirmPopupWidget.h"
#include "Widget/Ingame/WC_PopupWidget.h"
#include "Widget/Ingame/WC_BlackSmithWidget.h"
#include "Widget/Ingame/WC_ItemMakerWidget.h"
#include "Widget/Ingame/WC_LoadingWidget.h"

#include "Widget/Chat/WC_ChatMessageWidget.h"
#include "Widget/Chat/WC_ChatWidget.h"

#include "Widget/Inventory/WC_InventoryWidget.h"
#include "Widget/Inventory/WC_EquipmentWidget.h"
#include "Widget/Inventory/WC_RemoveWidget.h"
#include "Widget/Inventory/WC_ItemOptionWidget.h"

#include "Widget/Skill/WC_PlayerSkillWidget.h"
#include "Widget/Setting/WC_SettingWidget.h"

#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Data/DA_ItemDataList.h"
#include "Data/Recipe/DA_RecipeDataList.h"

void AC_WorldHUD::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(EInputMode::GameOnly, false);

	// 플레이어 위젯 생성 
}

void AC_WorldHUD::SetMyInfoWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!MyInfoWidget || !MyInfoWidget->IsInViewport())
		{
			CreateMyWidget(MyInfoWidgetClass, MyInfoWidget);
		}
	}
	else
	{
		if (MyInfoWidget)
		{
			MyInfoWidget->RemoveFromViewport();
		}
	}

	SetMouseMode(bOnShow);
}

void AC_WorldHUD::SetSettingWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!WBPC_SettingWidget || !WBPC_SettingWidget->IsInViewport())
		{
			CreateMyWidget(SettingWidgetClass, WBPC_SettingWidget);
		}
	}
	else
	{
		if (WBPC_SettingWidget)
		{
			WBPC_SettingWidget->RemoveFromViewport();
		}
	}

	SetMouseMode(bOnShow);
}

void AC_WorldHUD::SetInventoryWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!InventoryWidget || !InventoryWidget->IsInViewport())
		{
			CreateMyWidget(InventoryWidgetClass, InventoryWidget);
			InventoryWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			SetInputMode(EInputMode::GameAndUI, true);
			CastWCharacter(GetMyCharacter())->bOnMouse = true;
		}
	}
	else
	{
		if (InventoryWidget)
		{
			InventoryWidget->RemoveFromViewport();
			
			if (InventoryWidget->BeginSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, InventoryWidget->BeginSound, GetActorLocation(), CastWPC(GetMyPC())->SoundManager->EffectVolume, 1.0f);
			}
		}

		SetEquipmentWidgetVisible(false);
	}

	SetMouseMode(bOnShow);
}

void AC_WorldHUD::SetEquipmentWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!EquipmentWidget || !EquipmentWidget->IsInViewport())
		{
			CreateMyWidget(EquipmentWidgetClass, EquipmentWidget);
			EquipmentWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (EquipmentWidget)
		{
			EquipmentWidget->RemoveFromViewport();
		}
	}

	SetMouseMode(bOnShow);
}

void AC_WorldHUD::SetMerchantWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!MerchantWidget || !MerchantWidget->IsInViewport())
		{
			CreateMyWidget(MerchantWidgetClass, MerchantWidget);
			MerchantWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		if (MerchantWidget)
		{
			MerchantWidget->RemoveFromViewport();
		}
	}

	SetVisibilityUnderUI(!bOnShow);
	SetMouseMode(bOnShow);
}

void AC_WorldHUD::SetSellAreaWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!SellAreaWidget || !SellAreaWidget->IsInViewport())
		{
			CreateMyWidget(SellAreaWidgetClass, SellAreaWidget);
			SellAreaWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetMouseMode(bOnShow);
		}
	}
	else
	{
		if (SellAreaWidget)
		{
			SellAreaWidget->RemoveFromViewport();
		}
	}
}

void AC_WorldHUD::SetShopConfirmWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!ShopConfirmWidget || !ShopConfirmWidget->IsInViewport())
		{
			CreateMyWidget(ShopConfirmWidgetClass, ShopConfirmWidget);
			ShopConfirmWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetMouseMode(bOnShow);
		}
	}
	else
	{
		if (ShopConfirmWidget)
		{
			ShopConfirmWidget->RemoveFromViewport();
		}
	}
}

void AC_WorldHUD::SetConfirmPopupWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!ConfirmPopupWidget || !ConfirmPopupWidget->IsInViewport())
		{
			CreateMyWidget(ConfirmPopupWidgetClass, ConfirmPopupWidget);
			ConfirmPopupWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetMouseMode(bOnShow);
		}
	}
	else
	{
		if (ConfirmPopupWidget)
		{
			ConfirmPopupWidget->RemoveFromViewport();
		}
	}
}

void AC_WorldHUD::SetRemoveWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!RemoveWidget || !RemoveWidget->IsInViewport())
		{
			CreateMyWidget(RemoveWidgetClass, RemoveWidget);
			RemoveWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetMouseMode(bOnShow);
		}
	}
	else
	{
		if (RemoveWidget)
		{
			RemoveWidget->RemoveFromViewport();
		}
	}
}

void AC_WorldHUD::SetItemOptionWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!ItemOptionWidget || !ItemOptionWidget->IsInViewport())
		{
			CreateMyWidget(ItemOptionWidgetClass, ItemOptionWidget);
			ItemOptionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			SetMouseMode(bOnShow);
		}
	}
	else
	{
		if (ItemOptionWidget)
		{
			ItemOptionWidget->RemoveFromViewport();
		}
	}
}

void AC_WorldHUD::SetPopupWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!PopupWidget|| !PopupWidget->IsInViewport())
		{
			CreateMyWidget(PopupWidgetClass, PopupWidget);
			SetMouseMode(bOnShow);
		}
	}
	else
	{
		if (PopupWidget)
		{
			PopupWidget->RemoveFromViewport();
		}
	}
}

void AC_WorldHUD::SetVisibilityUnderUI(bool bOnShow)
{
	if (!PlayerWidget) return;

	PlayerWidget->SetVisibilityUnderUI(bOnShow);
}

void AC_WorldHUD::SetChatWidgetVisible(bool bOnShow)
{
	if (!PlayerWidget && !PlayerWidget->ChatWidget) return;

	PlayerWidget->ChatWidget->SetChatWidgetVisibility(bOnShow);
}

void AC_WorldHUD::SetBlackSmithWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!BlackSmithWidget || !BlackSmithWidget->IsInViewport())
		{
			CreateMyWidget(BlackSmithWidgetClass, BlackSmithWidget);
		}
	}
	else
	{
		if (BlackSmithWidget)
		{
			BlackSmithWidget->RemoveFromViewport();
		}
	}
}

void AC_WorldHUD::SetItemMakerWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!ItemMakerWidget || !ItemMakerWidget->IsInViewport())
		{
			CreateMyWidget(ItemMakerWidgetClass, ItemMakerWidget);
		}
	}
	else
	{
		if (ItemMakerWidget)
		{
			ItemMakerWidget->RemoveFromViewport();
		}
	}

	SetMouseMode(bOnShow);
}

void AC_WorldHUD::ActivateChat()
{
	if (!PlayerWidget) return;

	PlayerWidget->ChatWidget->ActivateChat();
}

void AC_WorldHUD::AddChatMessage(const FString& Message)
{
	APlayerController* OwningPlayer = GetOwningPlayerController();
	
	if (OwningPlayer && ChatWidgetClass && PlayerWidget->ChatWidget && ChatMessageWidgetClass)
	{
		// 메시지 한 줄에 해당하는 위젯 만듦
		UWC_ChatMessageWidget* ChatMessageWidget = CreateWidget<UWC_ChatMessageWidget>(OwningPlayer, ChatMessageWidgetClass);

		if (ChatMessageWidget)
		{
			ChatMessageWidget->SetChatMessage(Message);
			PlayerWidget->ChatWidget->AddChatMessage(ChatMessageWidget);
		}
	}
}

void AC_WorldHUD::InitProgressBar()
{
	if (!PlayerWidget) return;

	PlayerWidget->InitProgressBar();
}

void AC_WorldHUD::UpdateProgressBar(EProgressBarType type, float percent)
{
	if (!PlayerWidget) return;

	PlayerWidget->UpdateProgressBar(type, percent);
}

void AC_WorldHUD::UpdateAllUI()
{
	if (!PlayerWidget) return;

	PlayerWidget->UpdateAllUI();
}

void AC_WorldHUD::AddQuestText(AC_SMSQuest* Quest)
{
	APlayerController* OwningPlayer = GetOwningPlayerController();

	if (OwningPlayer && QuestingWidgetClass && QuestingWidget && QuestTextWidgetClass)
	{
		// 메시지 한 줄에 해당하는 위젯 만듦
		UWC_QuestingTextWidget* QuestingTextWidget = CreateWidget<UWC_QuestingTextWidget>(OwningPlayer, QuestTextWidgetClass);

		if (QuestingTextWidget)
		{
			QuestingWidget->SetQuest(QuestingTextWidget);
			QuestingTextWidget->UpdateText(Quest);
		}
	}
}

void AC_WorldHUD::RemoveQuestText(int32 index)
{
	if (!QuestingWidget) return;
	
	QuestingWidget->RemoveQuest(index);
}

void AC_WorldHUD::AddResultText(FString nickName, float Damage, unsigned int Level,EPlayerJob Job)
{
	APlayerController* OwningPlayer = GetOwningPlayerController();

	if (OwningPlayer && ResultWidgetClass && ResultWidget && ResultCharacterWidgetClass)
	{
		// 메시지 한 줄에 해당하는 위젯 만듦
		UWC_ResultCharacterWidget* ResultDetailWidget = CreateWidget<UWC_ResultCharacterWidget>(OwningPlayer, ResultCharacterWidgetClass);

		if (ResultDetailWidget)
		{
			ResultWidget->SetCharacter(ResultDetailWidget);
			ResultDetailWidget->UpdateText(nickName,Damage,Level);
			ResultDetailWidget->UpdateImage(Job);
		}
	}
}

void AC_WorldHUD::SetWidgetFromData(UDA_ItemData* InItemData, EShopMode InShopMode)
{
	if (!InItemData) return;

	ShopConfirmWidget->SetWidgetFromData(InItemData, InShopMode);
}

void AC_WorldHUD::SetItemData(UDA_ItemData* InItemData, EShopMode InShopMode)
{
	if (!InItemData) return;

	ConfirmPopupWidget->SetItemData(InItemData, InShopMode);
}

void AC_WorldHUD::SetItemData(UDA_ItemData* InItemData)
{
	if (!InItemData) return;

	RemoveWidget->SetItemData(InItemData);
}

void AC_WorldHUD::SetNoticeText(const FText& text)
{
	if (!PopupWidget) return;

	PopupWidget->SetNoticeText(text);
}

void AC_WorldHUD::InitSellItemGrid(UDA_ItemDataList* ItemDataList)
{
	if (!SellAreaWidget) return;

	SellAreaWidget->InitSellItemGrid(ItemDataList);
}

void AC_WorldHUD::InitRecipeGrid(UDA_RecipeDataList* RecipeDataList)
{
	if (!ItemMakerWidget) return;

	ItemMakerWidget->InitRecipeScrollBox(RecipeDataList);
}

void AC_WorldHUD::StartCooldown(AC_SMSSkillINS* InSkillInstance)
{
	if (!PlayerWidget) return;

	PlayerWidget->HotbarWidget->StartCooldown(InSkillInstance);
}

void AC_WorldHUD::SetMouseMode(bool bOnMouse)
{
	if (bOnMouse)
	{
		SetInputMode(EInputMode::GameAndUI, true); // 마우스 보여주기
	}
	else
	{
		SetInputMode(EInputMode::GameOnly, false); // 마우스 숨기기	
	}
}

void AC_WorldHUD::SetLoading(bool bOnLoading,bool _OnDungeon)
{
	if (bOnLoading)
	{
		if (!LoadingWidget || !LoadingWidget->IsInViewport())
		{
			CreateMyWidget(LoadingWidgetClass, LoadingWidget);
			LoadingWidget->AddToViewport(100);
			LoadingWidget->Init(_OnDungeon);
			SetInputMode(EInputMode::UIOnly, false);
			AC_WorldPlayerController* WPC = CastWPC(GetMyPC());
			if (WPC)
			{
				WPC->Subsystem->RemoveMappingContext(WPC->AttackIMC);
				WPC->Subsystem->RemoveMappingContext(WPC->LookIMC);
				WPC->Subsystem->RemoveMappingContext(WPC->MoveIMC);
			}
			
		}
	}
	else
	{
		if (LoadingWidget)
		{
			LoadingWidget->RemoveFromViewport();
			SetInputMode(EInputMode::GameOnly, false);
			AC_WorldPlayerController* WPC = CastWPC(GetMyPC());
			if (WPC)
			{
				WPC->Subsystem->AddMappingContext(WPC->AttackIMC, 0);
				WPC->Subsystem->AddMappingContext(WPC->LookIMC, 0);
				WPC->Subsystem->AddMappingContext(WPC->MoveIMC, 0);
			}
		}
	}
}

void AC_WorldHUD::SetQuestWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!QuestWidget || !QuestWidget->IsInViewport())
		{
			CreateMyWidget(QuestWidgetClass, QuestWidget);
		}
	}
	else
	{
		QuestWidget->RemoveFromViewport();
	}

	SetMouseMode(bOnShow);
}

void AC_WorldHUD::SetPlayerSkillWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!PlayerSkillWidget || !PlayerSkillWidget->IsInViewport())
		{
			CreateMyWidget(PlayerSkillWidgetClass, PlayerSkillWidget);
			PlayerSkillWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		PlayerSkillWidget->RemoveFromViewport();
	}
}

void AC_WorldHUD::SetQuestText(AC_SMSQuest* Quest)
{
	if (Quest)
	{
		QuestWidget->SetText(Quest);
	}
}

void AC_WorldHUD::SetQuestingWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!QuestingWidget || !QuestingWidget->IsInViewport())
		{
			CreateMyWidget(QuestingWidgetClass, QuestingWidget);
			QuestingWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}
	else
	{
		QuestingWidget->RemoveFromViewport();
	}
}

void AC_WorldHUD::UpdateQuestingText( AC_SMSQuest* Quest, int32 Index)
{
	
	QuestingWidget->UpdateQuest(Quest, Index);
	
}

void AC_WorldHUD::SetResultWidgetVisible(bool bOnShow,FString Boss)
{
	if (bOnShow)
	{
		if (!ResultWidget || !ResultWidget->IsInViewport())
		{
			CreateMyWidget(ResultWidgetClass, ResultWidget);
			ResultWidget->SetText(Boss);
		}
	}
	else
	{
		ResultWidget->RemoveFromViewport();
	}
}