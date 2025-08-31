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

	// ���� ���� Ŭ������ ����� ��� ��ȯ
	if (!SlotWidgetClass) return;

	// ���� �迭�� �ʱ�ȭ
	SlotArray.Empty();
	InventoryGrid->ClearChildren();

	for (int32 i = 0; i < NumSlots; ++i)
	{
		// ���� ������ �����ϰ�, �׸��忡 �߰�
		UWC_ItemSlot* NewSlot = CreateWidget<UWC_ItemSlot>(this, SlotWidgetClass);
		InventoryGrid->AddChildToUniformGrid(NewSlot, i / 5, i % 5);
		
		// ������ ���� �ʱ�ȭ
		NewSlot->ClearSlot();
		NewSlot->SlotIndex = i;
		NewSlot->SetParentWidget(this);

		SlotArray.Add(NewSlot);
	}

	// InventoryComponent�� ��� Delegate ���ε�
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

			if (BeginSound)// �κ��丮 �Ҹ�
			{
				UGameplayStatics::PlaySoundAtLocation(this, BeginSound, Player->GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
			}
		}
	}

	// â �ݱ� ��ư �ʱ�ȭ
	CloseButton->Init(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Image/00_PictoIcons/Delete")), FVector3f(1.f, 1.f, 1.f));
	// â �ݱ� ��ư�� �Լ� ���ε�
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
	// �κ��丮 ������Ʈ�� ����ִ� ��� ��ȯ
	if (!InventoryComponent) return;

	// �κ��丮 ������Ʈ�� �����͸� �ҷ��ͼ� ����
	const TArray<FInventorySlot>& InventorySlots = InventoryComponent->GetInventoryData();

	for (int32 i = 0; i < SlotArray.Num(); ++i)
	{
		// ������ �����ͺ��� ���� ���� ���
		if (i < InventorySlots.Num())
		{
			// ������ �����ͷ� ����
			const FInventorySlot& SlotData = InventorySlots[i];

			SlotArray[i]->SetSlot(SlotData);
		}
		// ������ �����ͺ��� ���� ū ���
		else
		{
			// ������ ���
			SlotArray[i]->ClearSlot();
		}
	}

	TEXT_Gold->SetText(FText::AsNumber(InventoryComponent->Gold));
}

void UWC_InventoryWidget::OnCloseButtonClicked()
{
	// �� ����â �ݱ�
	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (WorldHUD)
	{
		WorldHUD->SetInventoryWidgetVisible(false);
	}
}

void UWC_InventoryWidget::SetShowOptionWidget(UWC_ItemSlot* InSlot, const FText& text)
{
	// �ɼ� ������ �ִٸ� �����ϱ�
	if (OptionWidget)
	{
		OptionWidget->RemoveFromParent();
		OptionWidget = nullptr;
	}

	// �ɼ� ���� ����
	OptionWidget = CreateWidget<UWC_ItemOptionWidget>(GetWorld(), OptionWidgetClass);

	if (!OptionWidget || !InSlot) return;

	// Ŭ���� ���� ��ġ�� �ɼ� ���� ��ġ ����
	FVector2D ScreenPos;
	UWidgetLayoutLibrary::GetMousePositionScaledByDPI(GetOwningPlayer(), ScreenPos.X, ScreenPos.Y);

	if (!MainCanvas) return;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(MainCanvas->AddChild(OptionWidget));

	if (!CanvasSlot) return;

	// ��ġ ����
	CanvasSlot->SetPosition(ScreenPos);

	// ������ ����
	OptionWidget->SetDesiredSizeInViewport(FVector2D(200.f, 100.f));
	OptionWidget->SetItemSlot(InSlot);
	OptionWidget->SetButtonText(text);
}