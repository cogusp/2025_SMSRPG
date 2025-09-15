// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/C_SMSPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "C_TitlePlayerController.generated.h"

class AC_PlayerCharacter;
/**
 * 
 */
UCLASS()
class STRONGMETALSTONE_API AC_TitlePlayerController : public AC_SMSPlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, Reliable)
	void ServerSpawnMyCharacter(const FString& _UserID);

	UFUNCTION(Client, Reliable)
	void ClientSpawnMyCharacter(const TArray<FCharacterInfo>& Characters);

	void ChangeCamera(ECameraViewPoint onFace);

private:
	virtual void BeginPlay()override;
	virtual void SetupInputComponent() override;

	void OnLeftMouseClick(const FInputActionValue& Value);

public:
	AC_PlayerCharacter* SelectCharacter;
private:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* TitleIMC; 

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = Input, Meta = (AllowPrivateAccess = "true"))
	class UInputAction* MouseLeftClickAction;

private:
	TArray<AC_PlayerCharacter*> SpawnCharacters;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<AC_PlayerCharacter> Warrior;

	UPROPERTY(EditDefaultsOnly, Category = "Character")
	TSubclassOf<AC_PlayerCharacter> Mage;

	AC_PlayerCharacter* CustomCharacter;
};

