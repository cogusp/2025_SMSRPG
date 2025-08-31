// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/C_NonPlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Character/C_PlayerCharacter.h"

AC_NonPlayerCharacter::AC_NonPlayerCharacter()
{
	InteractCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractCollision"));
	InteractCollision->SetupAttachment(RootComponent);
	InteractCollision->SetSphereRadius(200.0f);
	InteractCollision->SetCollisionProfileName(TEXT("NPC"));
}

void AC_NonPlayerCharacter::Action(AC_PlayerCharacter* Character)
{
}
