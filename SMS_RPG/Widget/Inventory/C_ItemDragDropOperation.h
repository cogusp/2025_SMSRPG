#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "C_ItemDragDropOperation.generated.h"

UCLASS()
class STRONGMETALSTONE_API UC_ItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	class UWC_Slot* ItemSlot;

	UPROPERTY()
	UTexture2D* ItemImage;
};
