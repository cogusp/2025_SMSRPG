#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrameWork.h"
#include "C_SMSPlayerController.generated.h"

UCLASS()
class STRONGMETALSTONE_API AC_SMSPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AC_SMSPlayerController();

	void BeginPlay()override;

	UFUNCTION(Server, Reliable)
	void ServerLoadUserInfoFromCSV(); //서버에서 유저 CSV를 읽는 함수

	UFUNCTION(Server, Reliable)
	void ServerLoadPlayerInfoFromCSV(); //서버에서 플레이어 CSV를 읽는 함수

	UFUNCTION(Server, Reliable)
	void ServerCheckIDFromCSV(const FString& _UserInfo); //서버에서 플레이어 CSV를 읽는 함수

	UFUNCTION(Client, Reliable)
	void ClientCheckIDFromCSV(); //클라에서 IDCheck

	UFUNCTION(Server, Reliable)
	void ServerAddCharacterInfoToCSV(FCharacterInfo _CharacterInfo); //서버에 캐릭터 추가함수

	UFUNCTION(NetMulticast, Reliable)
	void MultiLoadPlayerInfoFromCSV(const TArray<FCharacterInfo>& _CharacterInfoDatas); //받은 플레이어정보를 멀티로 뿌려주는 함수

	UFUNCTION(NetMulticast, Reliable)
	void MultiLoadUserInfoFromCSV(const TArray<FUserInfo>& _UserInfoDatas); //받은 유저정보를 멀티로 뿌려주는 함수


	UFUNCTION(Server, Reliable)
	void ServerDeleteCharacterInfoCSV(FCharacterInfo _CharacterInfo);//서버에서 캐릭터 삭제함수

	UFUNCTION(Client, Reliable)
	void ClientOpenLevel(int PortNum, FPartyInfo _PartyInfo);

	UFUNCTION(Server, Reliable)
	void ServerAddUserInfoToCSV(FUserInfo _UserInfo);

	UFUNCTION(Server, Reliable)
	void ServerSetIsExit(bool Exit);

public:
	UPROPERTY()
	bool IsExit = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manager")
	AC_SoundManager* SoundManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manager")
	TSubclassOf<AC_SoundManager> SoundManagerClass;
};
