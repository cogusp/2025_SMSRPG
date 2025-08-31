#include "Actor/Item/C_Item.h"
#include "Component/C_InventoryComponent.h"
#include "Character/C_PlayerCharacter.h"
#include "Data/DA_ItemData.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "Widget/Ingame/WC_ItemNameWidget.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"

AC_Item::AC_Item()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	// Collision 설정
	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	RootComponent = Collision;
	Collision->InitSphereRadius(200.f);
	Collision->SetCollisionProfileName(TEXT("Item"));
	// overlap 활성화 및 바인딩
	Collision->SetGenerateOverlapEvents(true);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AC_Item::OnBeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AC_Item::OnEndOverlap);

	// Mesh 설정
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Widget 설정
	NameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidget"));
	NameWidget->SetupAttachment(RootComponent);

	// 설정
	NameWidget->SetWidgetSpace(EWidgetSpace::Screen);
	NameWidget->SetDrawAtDesiredSize(true);
	NameWidget->SetVisibility(false);

	// 위젯 클래스 지정
	UClass* LoadedWidgetClass = LoadClass<UWC_ItemNameWidget>(nullptr, TEXT("/Game/BluePrint/Widget/Ingame/WBPC_ItemNameWidget.WBPC_ItemNameWidget_C"));
	
	if (LoadedWidgetClass)
	{
		NameWidget->SetWidgetClass(LoadedWidgetClass);
	}
}

void AC_Item::SetItemData(UDA_ItemData* InData, EItemType InType)
{
	ItemData = InData;
	ItemData->ItemType = InType;
}

void AC_Item::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<AC_PlayerCharacter>(OtherActor)) return;

	ShowNameWidget(true);
}

void AC_Item::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Cast<AC_PlayerCharacter>(OtherActor)) return;

	ShowNameWidget(false);
}

void AC_Item::OnPickup(AC_PlayerCharacter* Player)
{
	if (AC_PlayerCharacter* PlayerCharacter = CastWCharacter(Player))
	{
		if (auto* Inventory = PlayerCharacter->InventoryComponent)
		{			
			Inventory->AddItem(ItemData);

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

void AC_Item::ShowNameWidget(bool bOnShow)
{
	if (bOnShow)
	{
		if (UWC_ItemNameWidget* ItemWidget = Cast<UWC_ItemNameWidget>(NameWidget->GetUserWidgetObject()))
		{
			ItemWidget->SetTextName(FText::FromName(ItemData->ItemData.Name));
		}
	}


	NameWidget->SetVisibility(bOnShow);
}

void AC_Item::BeginPlay()
{
	Super::BeginPlay();

	AC_PlayerCharacter* Character = CastWCharacter(GetMyCharacter());
	AC_WorldPlayerController* WorldController = Cast<AC_WorldPlayerController>(Character->GetController());
	if (!WorldController)return;

	if (BeginSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, BeginSound, GetActorLocation(), WorldController->SoundManager->EffectVolume, 1.0f);
	}
}
