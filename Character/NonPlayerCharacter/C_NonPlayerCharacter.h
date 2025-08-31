// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/C_SMSCharacter.h"
#include "C_NonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class STRONGMETALSTONE_API AC_NonPlayerCharacter : public AC_SMSCharacter
{
	GENERATED_BODY()
	
public:
	AC_NonPlayerCharacter();

	virtual void Action(AC_PlayerCharacter* Character);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	class USphereComponent* InteractCollision; //상호작용 콜리젼박스
};
