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

    //Ŭ���̾�Ʈ���� ĳ���������� �޾ƿ� SpawnPlayer()�� ȣ���Ŵ
    CastWPC(NewPlayer)->ClientSpawnCharacter(CurPlayerController);

}


void AC_WorldGameMode::FindStartPoint()
{
    WorldPlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
}



void AC_WorldGameMode::SpawnPlayer(FCharacterInfo _CharacterInfo, APlayerController* PC)
{
    //���� ���н� �ߴ� �α�
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
    //�� ��ҿ� ���ļ� �����ɶ� ������ �����ǰ�
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


    switch (_CharacterInfo.Job)
    {
    case EPlayerJob::NONE:
        SpawnClass = TestCharacterClass; //ĳ���� �����Ҷ� �̰� �ٲ㼭�ϱ�, ���� ������ �ջ��Ű���ʰ� �ϱ� ����
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
    
    //��ó��
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
            //��Ƽ���϶� ��Ƽ���� �ٽ� �����ؾ���
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
                    //��Ƽ���϶� ��Ƽ���� �ٽ� �����ؾ���
                }

                Party.PartyPlayers.Remove(PartyCharacter);
                break;
            }
        }
    }

}



void AC_WorldGameMode::SendChatMessage(const FString& Message)
{
    // ��� �÷��̾� ��Ʈ�ѷ��� ���� Iterator �̿�
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        AC_WorldPlayerController* WorldPC = Cast<AC_WorldPlayerController>(*It);

        if (WorldPC)
        {
            WorldPC->ClientSendChat(Message);
        }
    }
}
