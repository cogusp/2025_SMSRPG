#pragma once

#include "CoreMinimal.h"
#include "PlayerController/C_WorldPlayerController.h"
#include "C_DungeonPlayerController.generated.h"


UCLASS()
class STRONGMETALSTONE_API AC_DungeonPlayerController : public AC_WorldPlayerController
{
	GENERATED_BODY()

protected:

	virtual void Ride(const FInputActionValue& Value)override; // ž��, ����
	virtual void BeginPlay()override;

};
