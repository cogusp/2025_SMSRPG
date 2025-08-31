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
	// 장착
	void TakeOn(int32 index);
	// 탈착
	void TakeOff(int32 index);

public:
	// 여러 슬롯을 통해 인벤토리 구성
	UPROPERTY(ReplicatedUsing = OnRep_EquipmentSlots)
	TArray<FInventorySlot> EquipmentSlots;

	// 인벤토리 크기
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 EquipmentSize = 7;

	// 소비 아이템 최대 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxItemQuantity = 99;

	// 장착한 장비가 변경될 때 사용할 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentUpdated OnEquipmentUpdate;
};
