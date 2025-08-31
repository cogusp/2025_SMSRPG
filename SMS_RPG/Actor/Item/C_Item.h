#pragma once

#include "CoreMinimal.h"
#include "Actor/C_SMSActor.h"
#include "C_Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UImage;
class AC_WorldPlayerController;
class UWC_ItemNameWidget;
class UWidgetComponent;

UCLASS()
class STRONGMETALSTONE_API AC_Item : public AC_SMSActor
{
	GENERATED_BODY()

public:
	AC_Item();

	virtual void SetItemData(UDA_ItemData* InData, EItemType InType);
	virtual const UDA_ItemData* GetItemData() const { return ItemData; }

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 플레이어가 아이템을 줍는 함수
	virtual void OnPickup(class AC_PlayerCharacter* Player);

	// NameWidget을 활성/비활성화하는 함수
	void ShowNameWidget(bool bOnShow);
protected:
	virtual void BeginPlay()override;

public:
	// 기본 설정
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collision;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UImage* Icon;

	// 아이템 데이터
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	UDA_ItemData* ItemData;

	// 오버랩 됐을 때 띄울 UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* NameWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* BeginSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* PickUpSound;
};
