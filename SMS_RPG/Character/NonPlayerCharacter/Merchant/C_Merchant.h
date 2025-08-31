#pragma once

#include "CoreMinimal.h"
#include "Character/C_NonPlayerCharacter.h"
#include "C_Merchant.generated.h"

class UDA_ItemDataList;

UCLASS()
class STRONGMETALSTONE_API AC_Merchant : public AC_NonPlayerCharacter
{
	GENERATED_BODY()
	
public:
	virtual void Action(AC_PlayerCharacter* Character) override;

public:
	UPROPERTY(EditAnywhere, Category = "Merchant")
	UDA_ItemDataList* DA_ItemDataList;
};