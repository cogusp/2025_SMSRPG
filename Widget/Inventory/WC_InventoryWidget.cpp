#include "Widget/Inventory/WC_InventoryWidget.h"
#include "Widget/Inventory/WC_ItemSlot.h"
#include "Widget/Inventory/WC_ItemOptionWidget.h"
#include "Widget/WC_SMSButton.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Component/C_InventoryComponent.h"
#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Data/DA_ItemData.h"
#include "HUD/C_WorldHUD.h"

void UWC_InventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 슬롯 위젯 클래스가 비었을 경우 반환
	if (!SlotWidgetClass) return;

	// 슬롯 배열을 초기화
	SlotArray.Empty();
	InventoryGrid->ClearChildren();

	for (int32 i = 0; i < NumSlots; ++i)
	{
		// 슬롯 위젯을 생성하고, 그리드에 추가
		UWC_ItemSlot* NewSlot = CreateWidget<UWC_ItemSlot>(this, SlotWidgetClass);
		InventoryGrid->AddChildToUniformGrid(NewSlot, i / 5, i % 5);
		
		// 생성한 슬롯 초기화
		NewSlot->ClearSlot();
		NewSlot->SlotIndex = i;
		NewSlot->SetParentWidget(this);

		SlotArray.Add(NewSlot);
	}

	// InventoryComponent를 얻어 Delegate 바인딩
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AC_PlayerCharacter* Player = CastWCharacter(PC->GetPawn()))
		{
			InventoryComponent = Player->GetInventoryComponent();

			if (InventoryComponent)
			{
				InventoryComponent->OnInventoryUpdate.AddDynamic(this, &UWC_InventoryWidget::RefreshInventory);
				TEXT_Gold->SetText(FText::AsNumber(InventoryComponent->Gold));
			}

			AC_WorldPlayerController* WorldController = Cast<AC_WorldPlayerController>(Player->GetController());
			if (!WorldController)return;

			if (BeginSound)// 인벤토리 소리
			{
				UGameplayStatics::PlaySoundAtLocation(this, BeginSound, Player->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
			}
		}
	}

	// 창 닫기 버튼 초기화
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// 창 닫기 버튼에 함수 바인딩
	CloseButton->OnButtonClicked.AddDynamic(this, &UWC_InventoryWidget::OnCloseButtonClicked);

	RefreshInventory();
}

FReply UWC_InventoryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!OptionWidget) return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);;

	if (OptionWidget->GetParent())
	{
		OptionWidget->RemoveFromParent();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UWC_InventoryWidget::RefreshInventory()
{
	// 인벤토리 컴포넌트가 비어있는 경우 반환
	if (!InventoryComponent) return;

	// 인벤토리 컴포넌트의 데이터를 불러와서 저장
	const TArray<FInventorySlot>& InventorySlots = InventoryComponent->GetInventoryData();

	for (int32 i = 0; i < SlotArray.Num(); ++i)
	{
		// 가져온 데이터보다 수가 작은 경우
		if (i < InventorySlots.Num())
		{
			// 가져온 데이터로 설정
			const FInventorySlot& SlotData = InventorySlots[i];

			SlotArray[i]->SetSlot(SlotData);
		}
		// 가져온 데이터보다 수가 큰 경우
		else
		{
			// 슬롯을 비움
			SlotArray[i]->ClearSlot();
		}
	}

	TEXT_Gold->SetText(FText::AsNumber(InventoryComponent->Gold));
}

void UWC_InventoryWidget::OnCloseButtonClicked()
{
	// 내 정보창 닫기
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetInventoryWidgetVisible(false);
	}
}

void UWC_InventoryWidget::SetShowOptionWidget(UWC_ItemSlot* InSlot, const FText& text)
{
	// 옵션 위젯이 있다면 삭제하기
	if (OptionWidget)
	{
		OptionWidget->RemoveFromParent();
		OptionWidget = nullptr;
	}

	// 옵션 위젯 생성
	OptionWidget = CreateWidget<UWC_ItemOptionWidget>(GetWorld(), OptionWidgetClass);

	if (!OptionWidget || !InSlot) return;

	// 클릭한 슬롯 위치에 옵션 위젯 위치 변경
	FVector2D ScreenPos;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), ScreenPos.X, ScreenPos.Y);

	if (!MainCanvas) return;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MainCanvas->AddChild(OptionWidget));

	if (!CanvasSlot) return;

	// 위치 설정
	CanvasSlot->SetPosition(ScreenPos);

	// 사이즈 설정
	OptionWidget->SetDesiredSizeInViewport(FVector2D(200.f, 100.f));
	OptionWidget->SetItemSlot(InSlot);
	OptionWidget->SetButtonText(text);
}