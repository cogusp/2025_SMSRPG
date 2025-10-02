#include "GameMode/C_WorldGameMode.h"
#include "C_SMSGameInstance.h"
#include "Character/C_PlayerCharacter.h"
#include "Component/C_PartySystemComponent.h"
#include "PlayerController/C_WorldPlayerController.h"

void AC_WorldGameMode::BeginPlay()
{
    Super::BeginPlay();
}

void AC_WorldGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    FindStartPoint();

    CurPlayerController = NewPlayer;

    //클라이언트에서 캐릭터정보를 받아와 SpawnPlayer()를 호출시킴
    CastWPC(NewPlayer)->ClientSpawnCharacter(CurPlayerController);

}


void AC_WorldGameMode::FindStartPoint()
{
    WorldPlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
}



void AC_WorldGameMode::SpawnPlayer(FCharacterInfo _CharacterInfo, APlayerController* PC)
{
    //생성 실패시 뜨는 로그
    if (!CurPlayerController)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnPlayer: NewPlayer is null"));
        return;
    }

    if (!WorldPlayerStart)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnPlayer: WorldPlayerStart is null"));
        return;
    }

    FActorSpawnParameters SpawnParams;
    UClass* SpawnClass = TestCharacterClass;
    //한 장소에 곂쳐서 생성될때 옆에서 생성되게
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


    switch (_CharacterInfo.Job)
    {
    case EPlayerJob::NONE:
        SpawnClass = TestCharacterClass; //캐릭터 실험할때 이거 바꿔서하기, 원래 로직을 손상시키지않고 하기 위해
        break;
    case EPlayerJob::WARRIOR:
        SpawnClass = Warrior;
        break;
    case EPlayerJob::MAGE:
        SpawnClass = Mage;
        break;
    case EPlayerJob::HEALER:
        SpawnClass = Healer;
        break;
    default:
        break;
    }
    float TempFloat = UKismetMathLibrary::RandomFloatInRange(-500.0f, 500.0f);
    float TempFloat2 = UKismetMathLibrary::RandomFloatInRange(-500.0f, 500.0f);

    FTransform TempTransform = WorldPlayerStart->GetTransform();
    TempTransform.SetLocation(FVector(TempTransform.GetLocation().X + Players.Num()*50+ TempFloat, TempTransform.GetLocation().Y + Players.Num() * 50+ TempFloat2, TempTransform.GetLocation().Z+200));
    
    //후처리
    CurPlayer = GetWorld()->SpawnActor<AC_PlayerCharacter>(SpawnClass, TempTransform, SpawnParams);
    CurPlayer->CharacterInfo = _CharacterInfo;

    PC->Possess(CurPlayer);

    Characters.Add(CurPlayer);
    Players.Add(PC);

    for (int i = 0; i < Players.Num(); ++i)
    {
        AC_WorldPlayerController* WPC = Cast< AC_WorldPlayerController>(Players[i]);
        if (WPC->HorseINS)
        {
            WPC->RidingOwner->ServerPlayMontage(WPC->RidingOwner->RideMontage);
        }
    }
}

void AC_WorldGameMode::LogOut(APlayerController* _NewPlayer, AC_PlayerCharacter* _Character)
{
    Characters.Remove(_Character);
    Players.Remove(_NewPlayer);

    if (!CastWPC(_NewPlayer)->IsExit)
    {
        return;
    }

    for (AC_PlayerCharacter*& PartyCharacter : _Character->PartyInfo.PartyPlayers)
    {
        if (PartyCharacter == _Character) continue;

        if (_Character == PartyCharacter->PartyInfo.PartyOwner)
        {
            PartyCharacter->PartyInfo.PartyOwner = PartyCharacter;
            //파티장일때 파티장을 다시 지정해야함
        }

        PartyCharacter->PartyInfo.PartyPlayers.Remove(_Character);
        PartyCharacter->PartySytemComponent->ClientUpdateParty(PartyCharacter->PartyInfo);

    }

    for (FPartyInfo Party : PartyDatas)
    {
        for (AC_PlayerCharacter* PartyCharacter : Party.PartyPlayers)
        {
            if (PartyCharacter == _Character)
            {
                if (PartyCharacter == PartyCharacter->PartyInfo.PartyOwner)
                {
                    Party.PartyOwner = PartyCharacter;
                    //파티장일때 파티장을 다시 지정해야함
                }

                Party.PartyPlayers.Remove(PartyCharacter);
                break;
            }
        }
    }

}



void AC_WorldGameMode::SendChatMessage(const FString& Message)
{
    // 모든 플레이어 컨트롤러에 대한 Iterator 이용
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AC_WorldPlayerController* WorldPC = Cast<AC_WorldPlayerController>(*It);

        if (WorldPC)
        {
            WorldPC->ClientSendChat(Message);
        }
    }
}
