// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/C_DungeonPlayerController.h"

void AC_DungeonPlayerController::Ride(const FInputActionValue& Value)
{
}

void AC_DungeonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if (IsLocalController())
	//	SoundManager->PlayBGM(SoundManager->BGM_Boss); // 원하는 BGM 지정
}
