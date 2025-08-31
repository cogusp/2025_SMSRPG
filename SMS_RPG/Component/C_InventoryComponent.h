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

	// ���� �ε��� ��ȯ �Լ�
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
	// ���� ������ ���� �κ��丮 ����
	UPROPERTY(ReplicatedUsing = OnRep_InventorySlots)
	TArray<FInventorySlot> InventorySlots;

	// �κ��丮 ũ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 InventorySize = 30;

	// �Һ� ������ �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxItemQuantity = 99;

	// ������
	UPROPERTY()
	int32 Gold = 0;

	// �κ��丮�� ����� �� ����� ��������Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdate;
};