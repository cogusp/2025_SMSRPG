#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DA_ItemDataList.generated.h"

class UDA_ItemData;

UCLASS()
class STRONGMETALSTONE_API UDA_ItemDataList : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item List")
	TArray<UDA_ItemData*> ItemList;
};
