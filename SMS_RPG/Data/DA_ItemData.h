#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FrameWork.h"
#include "DA_ItemData.generated.h"

UCLASS()
class STRONGMETALSTONE_API UDA_ItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemType ItemType = EItemType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemSubType ItemSubType = EItemSubType::NONE;
};