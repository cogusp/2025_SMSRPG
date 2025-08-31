#include "Widget/Inventory/WC_ItemOptionWidget.h"
#include "Widget/Inventory/WC_ItemSlot.h"
#include "Widget/WC_SMSButton.h"

#include "Component/C_InventoryComponent.h"
#include "HUD/C_WorldHUD.h"
#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"

void UWC_ItemOptionWidget::NativeConstruct()
{
	//Super::NativeConstruct();

	if (GetOwningPlayerPawn())
	{
		// 컴포넌트 저장
		if (AC_PlayerCharacter* Player = Cast<AC_PlayerCharacter>(GetOwningPlayerPawn()))
		{
			InventoryComponent = Player->GetInventoryComponent();
		}
	}

	// 버튼 초기화
	BTN_Remove->Init(FText::FromString(TEXT("버리기")), 12);
	// 창 닫기 버튼에 함수 바인딩
	BTN_Use->OnButtonClicked.AddDynamic(this, &UWC_ItemOptionWidget::OnUseButtonClicked);
	BTN_Remove->OnButtonClicked.AddDynamic(this, &UWC_ItemOptionWidget::OnRemoveButtonClicked);

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (BeginSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, BeginSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}

void UWC_ItemOptionWidget::OnUseButtonClicked()
{
	// 사용
	InventoryComponent->UseItem(ItemSlot->SlotData);

	// 위젯 지우기
	RemoveFromParent();

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (UseSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, UseSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}

void UWC_ItemOptionWidget::OnRemoveButtonClicked()
{
	// 버리기
	if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
	{
		WorldHUD->SetRemoveWidgetVisible(true);
		WorldHUD->SetItemData(ItemSlot->SlotData.ItemData);
	}

	// 위젯 지우기
	RemoveFromParent();

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = CastWPC(GetMyPC());
	if (!WorldController)return;

	if (RemoveSound)// 인벤토리 소리
	{
		UGameplayStatics::PlaySoundAtLocation(this, RemoveSound, Character->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}

void UWC_ItemOptionWidget::SetButtonText(const FText& text)
{
	if (!BTN_Use) return;

	BTN_Use->Init(text, 12);
}