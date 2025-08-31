#include "Actor/Item/C_Gold.h"
#include "Character/C_PlayerCharacter.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Component/C_InventoryComponent.h"
#include "Data/DA_GoldData.h"


void AC_Gold::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	Super::SetItemData(InData, InType);

	// DA_Potion으로 형변환
	ItemData = Cast<UDA_GoldData>(InData);

	if (!ItemData) return;

	ItemData->ItemType = InType;

	// Mesh, Icon 등 설정
	Mesh->SetStaticMesh(ItemData->Mesh);
}

void AC_Gold::OnPickup(AC_PlayerCharacter* Player)
{
	if (AC_PlayerCharacter* PlayerCharacter = CastWCharacter(Player))
	{
		if (auto* Inventory = PlayerCharacter->FindComponentByClass<UC_InventoryComponent>())
		{
			Inventory->AddGold(Amount);

			if (AC_WorldPlayerController* PC = Cast<AC_WorldPlayerController>(PlayerCharacter->GetController()))
			{
				if (PickUpSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, PickUpSound, GetActorLocation(), PC->SoundManager->EffectVolume, 1.0f);
				}
				PC->ServerDestroyItem(this);
			}

			SetActorEnableCollision(false);
		}
	}
}

void AC_Gold::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemData != nullptr) SetItemData(ItemData, EItemType::NONE);  // 이걸 통해 Mesh 세팅 등 처리
}

void AC_Gold::MulitySetAmount_Implementation(int32 _Value)
{
	Amount = _Value;
}
