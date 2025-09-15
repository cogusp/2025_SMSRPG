// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/C_TitlePlayerController.h"
#include "InputActionValue.h"
#include "C_SMSGameInstance.h"
#include "Character/C_PlayerCharacter.h"
#include "HUD/C_TitleHUD.h"
#include "Manager/C_CustomizingManager.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"

void AC_TitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_Standalone)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(GetLevelIP(6773)));
	}
	else
	{
		if (IsLocalController())
		SoundManager->PlayBGM(SoundManager->BGM_Title); // 원하는 BGM 지정			
	}
}

void AC_TitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(TitleIMC, 0);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(MouseLeftClickAction, ETriggerEvent::Triggered, this,&AC_TitlePlayerController::OnLeftMouseClick );
	}

}

void AC_TitlePlayerController::ServerSpawnMyCharacter_Implementation(const FString& _UserID)
{
	CastWGI(GetMyGI())->CharacterInfoDatas = GetLoadCharacterInfoFromCSV();

	TArray<FCharacterInfo> MyCharacters;

	for (FCharacterInfo character : CastWGI(GetMyGI())->CharacterInfoDatas)
	{
		//아이디체크
		if (character.UserID == _UserID)
		{
			MyCharacters.Add(character);
		}
	}
	ClientSpawnMyCharacter(MyCharacters);
}

void AC_TitlePlayerController::ClientSpawnMyCharacter_Implementation(const TArray<FCharacterInfo>& Characters)
{
	for (AC_PlayerCharacter* SpawnCharacter : SpawnCharacters)
	{
		GetWorld()->DestroyActor(SpawnCharacter);
	}
	SpawnCharacters.Reset();

	int i = 0;
	for (FCharacterInfo MyCharacter : Characters)
	{
		AC_PlayerCharacter* NewCharacter;

		switch (MyCharacter.Job)
		{
		case EPlayerJob::WARRIOR:
			NewCharacter = GetWorld()->SpawnActor<AC_PlayerCharacter>(Warrior);
			break;
		case EPlayerJob::MAGE:
			NewCharacter = GetWorld()->SpawnActor<AC_PlayerCharacter>(Mage);
			break;
		default:
			NewCharacter = GetWorld()->SpawnActor<AC_PlayerCharacter>(Warrior);
			break;
		}
		switch (i)
		{
		case 0:
			NewCharacter->SetActorLocation(FVector(-3350.0f, -1200.0f, 640.0f));
			break;
		case 1:
			NewCharacter->SetActorLocation(FVector(-3600.0f, -1300.0f, 620.0f));
			break;
		case 2:
			NewCharacter->SetActorLocation(FVector(-3850.0f, -1400.0f, 620.0f));
			break;
		default:
			break;
		}
		NewCharacter->CharacterInfo = MyCharacter;


		SpawnCharacters.Add(NewCharacter);

		NewCharacter->SetActorRotation(FRotator(0.0f, -90.0f, 0.0f));
		UC_CustomizingManager::SetCustom(NewCharacter, MyCharacter.CustomizingInfo);
		i++;
	}

}
void AC_TitlePlayerController::OnLeftMouseClick(const FInputActionValue& Value)
{
	FVector Start;
	FVector Dir;
	FHitResult HitResult;

	DeprojectMousePositionToWorld(Start, Dir);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery9);

	UKismetSystemLibrary::LineTraceSingleForObjects
	(
		this,
		Start,
		Start + (Dir * 4000.0f),
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	//전에 선택된 캐릭터 외곽선제거
	if (!HitResult.GetActor())
	{
		if (SelectCharacter)
		{
			SelectCharacter->GetMesh()->SetCustomDepthStencilValue(0);
			SelectCharacter = NULL;
		}
		return;
	}

	if (SelectCharacter)
		SelectCharacter->GetMesh()->SetCustomDepthStencilValue(0);




	SelectCharacter = Cast<AC_PlayerCharacter>(HitResult.GetActor());
	SelectCharacter->GetMesh()->SetCustomDepthStencilValue(1);

	switch (SelectCharacter->CharacterInfo.Job) //몽타주 재생
	{
	case EPlayerJob::WARRIOR:
		break;

	case EPlayerJob::MAGE:
		break;

	case EPlayerJob::HEALER:
		break;
	default:
		break;
	}
}

void AC_TitlePlayerController::ChangeCamera(ECameraViewPoint onFace)
{
	CustomCharacter = Cast<AC_TitleHUD>(GetMyHUD())->CustomChar;

	if (onFace==ECameraViewPoint::Character)
	{
		SetViewTargetWithBlend(CustomCharacter, 1.0f);
		CustomCharacter->CustomCamera->SetActive(true);
		CustomCharacter->ThirdPersonCamera->SetActive(false);
	}
	else if(onFace==ECameraViewPoint::Pawn)
	{
		if (APawn* OriginalPawn = GetPawn())
		{
			SetViewTargetWithBlend(OriginalPawn, 1.0f);
		}
	}
	else if (onFace == ECameraViewPoint::Bag)
	{
		TArray<AActor*> FoundCameras;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundCameras);

		for (AActor* Camera : FoundCameras)
		{
			if (Camera->ActorHasTag(FName("Bag")))
			{
				SetViewTargetWithBlend(Camera, 1.0f);
				return;
			}
		}
	}
	
}
