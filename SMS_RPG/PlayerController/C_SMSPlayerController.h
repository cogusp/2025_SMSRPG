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
	void ServerLoadUserInfoFromCSV(); //�������� ���� CSV�� �д� �Լ�

	UFUNCTION(Server, Reliable)
	void ServerLoadPlayerInfoFromCSV(); //�������� �÷��̾� CSV�� �д� �Լ�

	UFUNCTION(Server, Reliable)
	void ServerCheckIDFromCSV(const FString& _UserInfo); //�������� �÷��̾� CSV�� �д� �Լ�

	UFUNCTION(Client, Reliable)
	void ClientCheckIDFromCSV(); //Ŭ�󿡼� IDCheck

	UFUNCTION(Server, Reliable)
	void ServerAddCharacterInfoToCSV(FCharacterInfo _CharacterInfo); //������ ĳ���� �߰��Լ�

	UFUNCTION(NetMulticast, Reliable)
	void MultiLoadPlayerInfoFromCSV(const TArray<FCharacterInfo>& _CharacterInfoDatas); //���� �÷��̾������� ��Ƽ�� �ѷ��ִ� �Լ�

	UFUNCTION(NetMulticast, Reliable)
	void MultiLoadUserInfoFromCSV(const TArray<FUserInfo>& _UserInfoDatas); //���� ���������� ��Ƽ�� �ѷ��ִ� �Լ�


	UFUNCTION(Server, Reliable)
	void ServerDeleteCharacterInfoCSV(FCharacterInfo _CharacterInfo);//�������� ĳ���� �����Լ�

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
