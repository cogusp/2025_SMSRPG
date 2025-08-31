#pragma once

#include "CoreMinimal.h"
#include "Component/C_SMSActorComponent.h"
#include "C_EquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentUpdated);

UCLASS()
class STRONGMETALSTONE_API UC_EquipmentComponent : public UC_SMSActorComponent
{
	GENERATED_BODY()
	
public:
	UC_EquipmentComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(int32 SlotIndex, int32 Count = 1);

	UFUNCTION(BlueprintCallable)
	void UseItem(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	TArray<FInventorySlot>& GetEquipmentData() { return EquipmentSlots; }

	UFUNCTION(BlueprintCallable)
	FInventorySlot GetInEquipmentData(int32 index) { return EquipmentSlots[index]; }

	UFUNCTION(Server, Reliable)
	void ServerUpdateEquipmentSlot(int32 Index, FInventorySlot InItemData);

	UFUNCTION()
	void OnRep_EquipmentSlots();

private:
	// ����
	void TakeOn(int32 index);
	// Ż��
	void TakeOff(int32 index);

public:
	// ���� ������ ���� �κ��丮 ����
	UPROPERTY(ReplicatedUsing = OnRep_EquipmentSlots)
	TArray<FInventorySlot> EquipmentSlots;

	// �κ��丮 ũ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EquipmentSize = 7;

	// �Һ� ������ �ִ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxItemQuantity = 99;

	// ������ ��� ����� �� ����� ��������Ʈ
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentUpdated OnEquipmentUpdate;
};
