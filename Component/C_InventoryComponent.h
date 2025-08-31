#pragma once

#include "CoreMinimal.h"
#include "Component/C_SMSActorComponent.h"
#include "C_InventoryComponent.generated.h"

class UDA_ItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STRONGMETALSTONE_API UC_InventoryComponent : public UC_SMSActorComponent
{
	GENERATED_BODY()

public:
	UC_InventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintCallable)
	bool AddItem(UDA_ItemData* NewItem, int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(UDA_ItemData* NewItem, int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	void UseItem(FInventorySlot NewSlot);

	// 슬롯 인덱스 변환 함수
	int32 GetEquipmentIndex(EItemType InItemType);


	UFUNCTION(BlueprintCallable)
	void AddGold(int32 Count);

	UFUNCTION(BlueprintCallable)
	const TArray<FInventorySlot>& GetInventoryData() const { return InventorySlots; }

	UFUNCTION(BlueprintCallable)
	int32 GetGold() const { return Gold; }

	UFUNCTION(Server, Reliable)
	void ServerUpdateInventorySlot(int32 Index, FInventorySlot InItemData);

	UFUNCTION()
	void OnRep_InventorySlots();
	
public:
	// 여러 슬롯을 통해 인벤토리 구성
	UPROPERTY(ReplicatedUsing = OnRep_InventorySlots)
	TArray<FInventorySlot> InventorySlots;

	// 인벤토리 크기
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InventorySize = 30;

	// 소비 아이템 최대 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxItemQuantity = 99;

	// 소지금
	UPROPERTY()
	int32 Gold = 0;

	// 인벤토리가 변경될 때 사용할 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdate;
};