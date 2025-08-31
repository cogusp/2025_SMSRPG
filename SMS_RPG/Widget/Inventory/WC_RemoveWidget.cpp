#include "Widget/Inventory/WC_RemoveWidget.h"
#include "Widget/WC_SMSButton.h"

#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

#include "Data/DA_ItemData.h"
#include "HUD/C_WorldHUD.h"
#include "Component/C_InventoryComponent.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Character/C_PlayerCharacter.h"

void UWC_RemoveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 창 닫기 버튼 초기화
	BTN_Yes->Init(FText::FromString(TEXT("결정")), 14);
	BTN_No->Init(FText::FromString(TEXT("취소")), 14);

	// 창 닫기 버튼에 함수 바인딩
	BTN_Yes->OnButtonClicked.AddDynamic(this, &UWC_RemoveWidget::OnYesButtonClicked);
	BTN_No->OnButtonClicked.AddDynamic(this, &UWC_RemoveWidget::OnNoButtonClicked);

	if (!TEXT_ItemCount || !TEXT_ItemName) return;

	// 텍스트 입력 시 실시간으로 변동을 확인할 함수 바인딩
	TEXT_ItemCount->OnTextChanged.AddDynamic(this, &UWC_RemoveWidget::OnTextChangedCallback);
	// 텍스트 초기화
	TEXT_ItemCount->SetText(FText::AsNumber(1));
	TEXT_ItemCount->SetFocus();
	PrevStr = "";

	// InventoryComponent를 얻어 Delegate 바인딩
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn()))
		{
			InventoryComponent = Player->GetInventoryComponent();
		}
	}
}

void UWC_RemoveWidget::OnTextChangedCallback(const FText& Text)
{
	FString InputStr = Text.ToString();

	// 숫자만 입력했는지 확인
	if (InputStr.IsEmpty() || InputStr.IsNumeric())
	{
		PrevStr = InputStr;
	}
	// 숫자가 아닌 경우 이전 값으로 되돌리기
	else
	{
		if (TEXT_ItemCount)
		{
			TEXT_ItemCount->SetText(FText::FromString(PrevStr));
		}
	}
}

void UWC_RemoveWidget::OnYesButtonClicked()
{
	if (!InventoryComponent) return;

	// 수량을 버튼 누른 시점의 텍스트를 직접 가져옴
	FString InputText = TEXT_ItemCount->GetText().ToString();
	int32 CountToRemove = FCString::Atoi(*InputText);

	// 입력한 수량이 0보다 적은 경우
	if (CountToRemove <= 0)
	{
		if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			WorldHUD->SetPopupWidgetVisible(true);
			WorldHUD->SetNoticeText(FText::FromString(TEXT("1개 이상의 수량을 입력하세요.")));
		}
		return;
	}

	// 플레이어가 가진 아이템 수가 버리려는 아이템 수보다 적은 경우 알림창
	if (!(InventoryComponent->RemoveItem(ItemData, CountToRemove)))
	{
		if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			WorldHUD->SetPopupWidgetVisible(true);
			WorldHUD->SetNoticeText(FText::FromString(TEXT("아이템이 부족합니다!")));
		}
		return;
	}

	// 위젯 다 지우기
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetRemoveWidgetVisible(false);
	}
}

void UWC_RemoveWidget::OnNoButtonClicked()
{
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetRemoveWidgetVisible(false);
	}
}

void UWC_RemoveWidget::SetItemData(UDA_ItemData* InItemData)
{
	if (!InItemData) return;

	ItemData = InItemData;

	TEXT_ItemName->SetText(FText::FromName(ItemData->ItemData.Name));
}