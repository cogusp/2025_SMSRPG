#include "PlayerController/C_SMSPlayerController.h"
#include "Character/C_PlayerCharacter.h"
#include "Component/C_PartySystemComponent.h"
#include "Manager/C_SoundManager.h"
#include "Widget/Title/WC_TitleWidget.h"
#include "HUD/C_TitleHUD.h"
#include "C_SMSGameInstance.h"

AC_SMSPlayerController::AC_SMSPlayerController()
{
}

void AC_SMSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//if (HasAuthority())
	//{
		SoundManager = GetWorld()->SpawnActor<AC_SoundManager>(SoundManagerClass);

		if (SoundManager)
		{
			SoundManager->Init();
		}
	/*}*/
	
}

void AC_SMSPlayerController::ServerCheckIDFromCSV_Implementation(const FString& _UserID)
{
	ServerLoadUserInfoFromCSV();
	for (FUserInfo UserInfoData : CastWGI(GetMyGI())->UserInfoDatas)
	{
		if (UserInfoData.ID == _UserID)
		{
			return;
		}
	}
	ClientCheckIDFromCSV();
}

void AC_SMSPlayerController::ClientCheckIDFromCSV_Implementation()
{
	Cast<AC_TitleHUD>(GetMyHUD())->TitleWidget->WBPC_LoginWidget->IDCheck = true;
}




//---------------------------------------------세이브로드---------------------------------------------


void AC_SMSPlayerController::ServerLoadUserInfoFromCSV_Implementation()
{
	CastWGI(GetMyGI())->SetLoadUserInfoFromCSV();
	MultiLoadUserInfoFromCSV(CastWGI(GetMyGI())->UserInfoDatas);
	
}
void AC_SMSPlayerController::ServerLoadPlayerInfoFromCSV_Implementation()
{
	CastWGI(GetMyGI())->CharacterInfoDatas = GetLoadCharacterInfoFromCSV();
	MultiLoadPlayerInfoFromCSV(CastWGI(GetMyGI())->CharacterInfoDatas);
}

void AC_SMSPlayerController::MultiLoadUserInfoFromCSV_Implementation(const TArray<FUserInfo>& _UserInfoDatas)
{
	CastWGI(GetMyGI())->UserInfoDatas = _UserInfoDatas;
}
void AC_SMSPlayerController::MultiLoadPlayerInfoFromCSV_Implementation(const TArray<FCharacterInfo>& _CharacterInfoDatas)
{
	CastWGI(GetMyGI())->CharacterInfoDatas = _CharacterInfoDatas;
}

void AC_SMSPlayerController::ServerAddUserInfoToCSV_Implementation(FUserInfo _UserInfo)
{
	CastWGI(GetMyGI())->SetLoadUserInfoFromCSV();
	CastWGI(GetMyGI())->CheckID(_UserInfo);
}

void AC_SMSPlayerController::ServerAddCharacterInfoToCSV_Implementation(FCharacterInfo _CharacterInfo)
{
	AddCharacterInfoToCSV(_CharacterInfo);
}

//---------------------------------------------레벨 이동---------------------------------------------
void AC_SMSPlayerController::ClientOpenLevel_Implementation(int PortNum, FPartyInfo _PartyInfo)
{
	if (HasAuthority()) return;
	CastWGI(GetMyGI())->PartyData = CastWCharacter(GetMyCharacter())->PartySytemComponent->PartyInfoToData(_PartyInfo);	

	//시간지나면 레벨이동
	FTimerHandle OpenLevelHandle;
	GetWorld()->GetTimerManager().SetTimer(OpenLevelHandle, FTimerDelegate::CreateLambda([=, this]()
		{
			UGameplayStatics::OpenLevel(GetWorld(), FName(*GetLevelIP(PortNum)));			
		}), 3.0f, false);
}

void AC_SMSPlayerController::ServerDeleteCharacterInfoCSV_Implementation(FCharacterInfo _CharacterInfo)
{
	DeleteCharacterInfoCSV(_CharacterInfo);
}

void AC_SMSPlayerController::ServerSetIsExit_Implementation(bool Exit)
{
	IsExit = Exit;
}
