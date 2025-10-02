#pragma once

#include "CoreMinimal.h"
#include "GameMode/C_SMSGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "C_WorldGameMode.generated.h"

UCLASS()
class STRONGMETALSTONE_API AC_WorldGameMode : public AC_SMSGameMode
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;	

	virtual void PostLogin(APlayerController* NewPlayer) override; //플레이어가 로그인시 호출되는 함수

	void FindStartPoint(); //시작지점 탐색	
public:
	void SpawnPlayer(FCharacterInfo _CharacterInfo, APlayerController* PC); //플레이어 생성

	virtual void LogOut(APlayerController* _NewPlayer, AC_PlayerCharacter* _Character);

	// 모든 클라이언트에게 메시지를 전달
	void SendChatMessage(const FString& Message);

public:
	UPROPERTY()
	TArray<APlayerController*> Players; //접속중인 플레이어컨트롤러들
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<AC_PlayerCharacter*> Characters; //접속중인 캐릭터들
	UPROPERTY()
	TArray<FPartyInfo> PartyDatas; //파티정보들


	APlayerStart* WorldPlayerStart; //플레이어 스타트

private:
	UPROPERTY()
	AC_PlayerCharacter* CurPlayer; //현재 접속시도중인 캐릭터
	UPROPERTY()
	APlayerController* CurPlayerController; //현재 접속시도중인 컨트롤러
};
