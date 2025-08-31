#include "Character/C_PlayerCharacter.h"
#include "Character/C_NonPlayerCharacter.h"
#include "Character/C_Enemy.h"
#include "Character/Riding/C_SMSRiding.h"

#include "GameState/C_FieldGameState.h"
#include "GameState/C_DungeonGameState.h"

#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Component/C_SkillSystemComponent.h"
#include "Component/C_PartySystemComponent.h"
#include "Component/C_ParticleSystemComponent.h"
#include "Component/C_InventoryComponent.h"
#include "Component/C_EquipmentComponent.h"

#include "Widget/Ingame/WC_CommunityWidget.h"
#include "Widget/Ingame/WC_PlayerWidget.h"
#include "Widget/Ingame/WC_EnemyInfoWidget.h"
#include "Widget/Ingame/WC_DeathWidget.h"
#include "Widget/Party/WC_InvitePartyWidget.h"

#include "GameMode/C_WorldGameMode.h"

#include "Manager/C_QuestManager.h"

#include "HUD/C_WorldHUD.h"
#include "Camera/CameraComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Net/UnrealNetwork.h"
#include "C_SMSGameInstance.h"

#include "PlayerController/C_WorldPlayerController.h"
#include "Actor/Item/C_Item.h"

#include "Math/Vector2D.h"
#include "Quest/C_SMSQuest.h"


AC_PlayerCharacter::AC_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetMesh()->SetRenderCustomDepth(true);

	GetCapsuleComponent()->InitCapsuleSize(CAPSULE_RADIUS, CAPSULE_HalfHEIGHT);	// 캡슐 사이즈
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));		// 콜리젼 타입
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AC_PlayerCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AC_PlayerCharacter::OnEndOverlap);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.0f); //회전속도
	GetCharacterMovement()->JumpZVelocity = 800.f;	// 점프 정도
	GetCharacterMovement()->MaxWalkSpeed = 600.f;	// 걷기 속도

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// 카메라 붐 창조
	CameraBoom->SetupAttachment(RootComponent);										// RootComponent 밑으로 넣기
	CameraBoom->TargetArmLength = 600.0f;											// 카메라 붐 길이 초기 설정
	CameraBoom->bUsePawnControlRotation = true;										

	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));						// 카메라 창조
	ThirdPersonCamera->SetupAttachment(CameraBoom);																	// 카메라 붐 밑으로 넣기
	ThirdPersonCamera->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 200.0f), FRotator(-20.0f, 0.0f, 0.0f));   // 카메라 위치 각도 초기설정
	ThirdPersonCamera->bUsePawnControlRotation = false;

	CustomBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CustomBoom"));	// 커스텀 카메라 붐 창조
	CustomBoom->SetupAttachment(RootComponent);
	CustomCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CustomCamera"));	// 커스텀 카메라 창조
	CustomCamera->SetupAttachment(CustomBoom);

	GetMesh()->SetReceivesDecals(false);
	GetCapsuleComponent()->SetReceivesDecals(false);

	// 미니맵 초기화
	InitMinimap();

	// 각종 시스템 컴포넌트 생성
	PartySytemComponent = CreateDefaultSubobject<UC_PartySystemComponent>(TEXT("PartySystemComponent"));
	ParticleSystemComponent = CreateDefaultSubobject<UC_ParticleSystemComponent>(TEXT("ParticleSystem"));
	InventoryComponent = CreateDefaultSubobject<UC_InventoryComponent>(TEXT("InventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UC_EquipmentComponent>(TEXT("EquipmentComponent"));

	// 뷰타겟 설정
	PreviousViewTarget = nullptr;
}

void AC_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetColor();
	//바로 LocallyControlled로 체크하면 빙의하기 전이라 딜레이를 주고 실행
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AC_PlayerCharacter::CheckLocalControl);

	// 사용할 Sprite 및 Material 저장
	MiniMapSprite = LoadObject<UPaperSprite>(nullptr, TEXT("/Game/Image/Minimap/Icon_PlayerInMiniMap_Sprite"));
	MiniMapMaterial = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/Image/Minimap/TranslucentUnlitSpriteMaterial.TranslucentUnlitSpriteMaterial"));

	// Sprite 및 Material 설정
	if (MiniMapIcon)
	{
		MiniMapIcon->SetSprite(MiniMapSprite);
		MiniMapIcon->SetMaterial(0, MiniMapMaterial);
	}

	MiniMapCameraBoom->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
}

void AC_PlayerCharacter::Init()
{
	Super::Init();
	
	ServerStopMontage();
	ServerInit();
	//기본 인풋모드가 없으면 추가
	if (!CastWPC(GetMyPC())->Subsystem->HasMappingContext(CastWPC(GetMyPC())->MoveIMC))
	{
		CastWPC(GetMyPC())->Subsystem->AddMappingContext(CastWPC(GetMyPC())->MoveIMC, 0);
	}
	if (!CastWPC(GetMyPC())->Subsystem->HasMappingContext(CastWPC(GetMyPC())->LookIMC))
	{
		CastWPC(GetMyPC())->Subsystem->AddMappingContext(CastWPC(GetMyPC())->LookIMC, 0);
	}
	if (!CastWPC(GetMyPC())->Subsystem->HasMappingContext(CastWPC(GetMyPC())->AttackIMC))
	{
		CastWPC(GetMyPC())->Subsystem->AddMappingContext(CastWPC(GetMyPC())->AttackIMC, 0);
	}
	CharacterInfo.Curhp = CharacterInfo.Maxhp;
	CharacterInfo.Curmp = 0;
	Absolute = false;
	bOnShield = false;
	CastWHUD(GetMyHUD())->UpdateAllUI();
}

void AC_PlayerCharacter::ServerInit_Implementation()
{
	APlayerStart* PlayerStart;
	PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	CharacterInfo.Curhp = CharacterInfo.Maxhp;
	CharacterInfo.Curmp = 0;
	Absolute = false;
	bOnShield = false;
	this->SetActorLocation(PlayerStart->GetActorLocation());
	MultiInit();
}

void AC_PlayerCharacter::MultiInit_Implementation()
{
	APlayerStart* PlayerStart;
	PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	CharacterInfo.Curhp = CharacterInfo.Maxhp;
	CharacterInfo.Curmp = 0;
	Absolute = false;
	bOnShield = false;

	this->SetActorLocation(PlayerStart->GetActorLocation());
}

void AC_PlayerCharacter::ServerSetTarget_Implementation(AC_SMSCharacter* Target)
{
	TargetCharacter = Target;
}

void AC_PlayerCharacter::CheckLocalControl()
{
	if (IsLocallyControlled())
	{
		if (SceneCaptureComponent)
		{
			SceneCaptureComponent->TextureTarget = LoadObject<UTextureRenderTarget2D>(nullptr, TEXT("/Game/Image/Minimap/BP_MiniMapRenderTarget"));
		}
		CharacterInfo = CastWGI(GetMyGI())->CharacterInfo;	

		CastWHUD(GetMyHUD())->CreateMyWidget(CastWHUD(GetMyHUD())->PlayerWidgetClass, CastWHUD(GetMyHUD())->PlayerWidget);
		ServerLoadQuest();

		
		Update();
		GetWorld()->GetTimerManager().ClearTimer(InitCheckTimerHandle);
	}
	else
	{
		// 빙의될때까지 시도
		GetWorld()->GetTimerManager().SetTimer(InitCheckTimerHandle, this, &AC_PlayerCharacter::CheckLocalControl, 0.1f, false);
	}
}

void AC_PlayerCharacter::IncreaseExp(unsigned int Exp)
{
	//경험치가 가능하는한 계속 레벨업 재귀적으로 호출
	if (Exp >= CharacterInfo.MaxExp)
	{
		Exp -= CharacterInfo.MaxExp;
		CharacterInfo.Level++;
		CharacterInfo.Maxhp += 500;
		CharacterInfo.Curhp = CharacterInfo.Maxhp;
		CharacterInfo.Damage += 10;
		CharacterInfo.Defense += 1;
		CharacterInfo.SkillPoint++;
		CharacterInfo.MaxExp = CastWGI(GetMyGI())->ExpDatas[CharacterInfo.Level];		
		IncreaseExp(Exp);
		
		return;
	}

	CharacterInfo.CurExp = Exp;
	CastWPC(GetMyPC())->ServerUpdateCharacter(CharacterInfo);	
	Update();
}

void AC_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GetMyCharacter() == this && !HasAuthority())
	FindTarget();

	// 아이콘 높이 고정
	if (MiniMapIcon)
	{
		MiniMapIcon->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, 1000.f));
	}
}

void AC_PlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AC_PlayerCharacter, TargetCharacter);
	DOREPLIFETIME(AC_PlayerCharacter, QuestArr);
	//DOREPLIFETIME(AC_PlayerCharacter, Absolute);
}

void AC_PlayerCharacter::Death(AC_SMSCharacter* Killer)
{
	Super::Death(Killer);
	Absolute = true;
	ClientDeath(Killer,this);
}

void AC_PlayerCharacter::InitMinimap()
{
	// 미니맵 내에서 보일 플레이어 아이콘
	// 미니맵의 카메라 붐
	MiniMapCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapCameraBoom"));
	MiniMapCameraBoom->SetupAttachment(RootComponent);
	MiniMapCameraBoom->TargetArmLength = 1000.f;

	// 카메라를 아래 방향으로 회전
	MiniMapCameraBoom->bUsePawnControlRotation = false;
	MiniMapCameraBoom->bInheritPitch = false;
	MiniMapCameraBoom->bInheritRoll = false;
	MiniMapCameraBoom->bInheritYaw = false;
	MiniMapCameraBoom->bDoCollisionTest = false;

	// 씬 캡쳐 (미니맵 카메라)
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(MiniMapCameraBoom);

	// 미니맵의 높이 고정
	SceneCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	SceneCaptureComponent->OrthoWidth = 15000.f;
	SceneCaptureComponent->CaptureSource = ESceneCaptureSource::SCS_SceneColorSceneDepth;
	SceneCaptureComponent->ShowFlags.SetSkeletalMeshes(false);
	SceneCaptureComponent->ShowFlags.SetParticles(false);
	SceneCaptureComponent->ShowFlags.SetSkyLighting(false);
	SceneCaptureComponent->ShowFlags.SetInstancedGrass(false);
	SceneCaptureComponent->ShowFlags.SetDynamicShadows(false);

	// 미니맵 내에서 보일 플레이어 아이콘
	MiniMapIcon = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("MiniMapIcon"));
	MiniMapIcon->SetupAttachment(RootComponent);						// 루트에 부착
	MiniMapIcon->SetRelativeLocation(FVector(0.f, 0.f, 1000.f));		// 머리 위 고정
	MiniMapIcon->SetUsingAbsoluteLocation(true);						// 월드 위치 고정
	MiniMapIcon->SetRelativeRotation(FRotator(0.f, -90.f, 90.f));		// 아이콘 회전
	MiniMapIcon->SetRelativeScale3D(FVector(1.5f));						// 아이콘 크기
	MiniMapIcon->SetUsingAbsoluteRotation(false);						// 회전 고정
	MiniMapIcon->SetVisibleInSceneCaptureOnly(true);					// SceneCapture에서만 보이도록 설정
}

void AC_PlayerCharacter::ClientDeath_Implementation(AC_SMSCharacter* Killer, AC_SMSCharacter* _MyCharacter)
{
	if (GetMyCharacter() != _MyCharacter) return;

	CastWHUD(GetMyHUD())->SetMouseMode(true);
	CastWHUD(GetMyHUD())->PlayerWidget->WBPC_DeathWidget->SetVisibility(ESlateVisibility::Visible);

	CastWPC(GetMyPC())->Subsystem->RemoveMappingContext(CastWPC(GetMyPC())->MoveIMC);
	CastWPC(GetMyPC())->Subsystem->RemoveMappingContext(CastWPC(GetMyPC())->LookIMC);
	CastWPC(GetMyPC())->Subsystem->RemoveMappingContext(CastWPC(GetMyPC())->AttackIMC);
}

void AC_PlayerCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// NPC일 때,
	if (AC_NonPlayerCharacter* NPC = Cast<AC_NonPlayerCharacter>(OtherActor))
	{
		OverlappedActor = OtherActor;
	}
	// Item일 때,
	else if (AC_Item* Item = Cast<AC_Item>(OtherActor))
	{
		OverlappedActor = OtherActor;
	}
}

void AC_PlayerCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlappedActor) OverlappedActor = nullptr;
}

void AC_PlayerCharacter::Update()
{
	ServerUpdate(CharacterInfo);

	GetWorld()->GetTimerManager().SetTimer(PartyTimerHandle, [this]()
		{
			ServerPartyUpdate(CastWGI(GetMyGI())->PartyData);
		}, 4.0f, false);  

	//ServerPartyUpdate(CastWGI(GetMyGI())->PartyData);

	if (!Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())) return;

	AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());	
	WorldHUD->UpdateAllUI();
}


void AC_PlayerCharacter::ServerUpdate_Implementation(FCharacterInfo _CharacterInfo)
{
	CharacterInfo = _CharacterInfo;           
	if (CharacterInfo.Curhp <= 0)
		CharacterInfo.Curhp = CharacterInfo.Maxhp;
	
	//이미 접속해있는 캐릭터 정보 뿌려주기
	for (AC_PlayerCharacter* TempCharacter : CastWGM(GetMyGM())->Characters)
	{
		//UE_LOG(LogTemp, Warning, TEXT("ServerCharactersNum : %d"), CastWGM(GetMyGM())->Characters.Num());
		//UE_LOG(LogTemp, Warning, TEXT("ServerCharacters : %s"), *TempCharacter->CharacterInfo.CharacterName);
		TempCharacter->MultiUpdate(TempCharacter->CharacterInfo);		
	}
}

void AC_PlayerCharacter::MultiUpdate_Implementation(FCharacterInfo _CharacterInfo)
{
	CharacterInfo = _CharacterInfo;
	if (CharacterInfo.Curhp <= 0)
	CharacterInfo.Curhp = CharacterInfo.Maxhp;
	UC_CustomizingManager::SetCustom(this, CharacterInfo.CustomizingInfo);

	if(!HasAuthority())
		CastWHUD(GetMyHUD())->UpdateAllUI();
}

void AC_PlayerCharacter::ClientUpdate_Implementation()
{	
	CastWHUD(GetMyHUD())->UpdateAllUI();
}

void AC_PlayerCharacter::ServerUpdateHP_Implementation(int32 ChangePoint)
{
	CharacterInfo.Curhp += CharacterInfo.Maxhp * (ChangePoint / 100.0f);

	if (CharacterInfo.Curhp > CharacterInfo.Maxhp)
		CharacterInfo.Curhp = CharacterInfo.Maxhp;

	//이미 접속해있는 캐릭터 정보 뿌려주기
	for (AC_PlayerCharacter* TempCharacter : CastWGM(GetMyGM())->Characters)
	{
		TempCharacter->MultiUpdateHP(ChangePoint);
		//여기에 경험치도
	}
}

void AC_PlayerCharacter::MultiUpdateHP_Implementation(int32 ChangePoint)
{
	CharacterInfo.Curhp += CharacterInfo.Maxhp * (ChangePoint / 100.0f);

	if (CharacterInfo.Curhp > CharacterInfo.Maxhp)
		CharacterInfo.Curhp = CharacterInfo.Maxhp;

	ClientUpdate();
}

void AC_PlayerCharacter::ServerPartyUpdate_Implementation(FPartyData _PartyData)
{
	TArray<AC_PlayerCharacter*> _Characters = CastWGM(GetMyGM())->Characters;

	if (_PartyData.InParty)
	{
		PartyInfo = PartySytemComponent->PartyDataToInfo(CastWGM(GetMyGM()), _PartyData);
	}
	//이미 접속해있는 캐릭터 정보 뿌려주기
	for (TWeakObjectPtr<AC_PlayerCharacter> TempCharacter : _Characters)
	{
		if(TempCharacter.IsValid())
		TempCharacter->MultiPartyUpdate(PartyInfo);
	}
}

void AC_PlayerCharacter::MultiPartyUpdate_Implementation(FPartyInfo _PartyInfo)
{	
	//파티원 위젯 업데이트
	for (AC_PlayerCharacter* TempCharacter : _PartyInfo.PartyPlayers)
	{
		if (TempCharacter == this)
		{
			PartyInfo = _PartyInfo;
			TempCharacter->PartySytemComponent->UpdateParty(PartyInfo);
		}
	}
}

void AC_PlayerCharacter::Move(FVector2D Value)
{
	FVector2D MovementVector = Value;

	if (Controller != nullptr)
	{
		if (bIsTopDownMode)
		{

			if (Value.IsNearlyZero()) return;

			APlayerController* PC = Cast<APlayerController>(GetController());
			if (!PC) return;
;
			FVector2D CharacterScreenLocation = FVector2D(1920, 1080);
			// 스크린 중심
			GetMyPC()->ProjectWorldLocationToScreen(this->GetActorLocation(), CharacterScreenLocation,true);

			// 입력 방향을 기준으로 오프셋 생성
			// 예: W = (0, -1), D = (1, 0) 등 
			float PixelOffset = 10.0f; // 오프셋 픽셀 값
			FVector2D Offset(Value.X * PixelOffset, -Value.Y * PixelOffset); // Y는 위가 0이므로 반전

			FVector2D TargetScreenPos = CharacterScreenLocation + Offset;

			// 스크린 좌표 → 월드 방향으로 변환
			FVector WorldOrigin, WorldDirection;
			if (UGameplayStatics::DeprojectScreenToWorld(PC, TargetScreenPos, WorldOrigin, WorldDirection))
			{
				// Z축 제거 (평면 이동만)
				WorldDirection.Z = 0;
				WorldDirection.Normalize();

				AddMovementInput(-WorldDirection, 1.0f);
			}
		}
		else
		{
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			AddMovementInput(ForwardDirection, MovementVector.X);
			AddMovementInput(RightDirection, MovementVector.Y);
		}
	}
}

void AC_PlayerCharacter::Look(FVector2D Value)
{
	FVector2D LookAxisVector = Value;

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AC_PlayerCharacter::InterActor(float Value)
{
	if (OverlappedActor)
	{
		// NPC인 경우
		if (AC_NonPlayerCharacter* NPC = Cast<AC_NonPlayerCharacter>(OverlappedActor))
		{
			NPC->Action(this);
		}
		// Item인 경우
		else if (AC_Item* Item = Cast<AC_Item>(OverlappedActor))
		{
			Item->OnPickup(this);
		}
		
		OverlappedActor = nullptr;
		SetCollisionActive();
	}
}

void AC_PlayerCharacter::OpenCommunity(float Value)
{
	FVector Start;
	FVector Dir;
	FHitResult HitResult;
	
	//화면에서 마우스방향으로 트레이스
	GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(Start, Dir);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery9);
	//ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);

	UKismetSystemLibrary::LineTraceSingleForObjects
	(
		this,
		Start,
		Start + (Dir * TRACE_COMMUNITY_DISTANCE),
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (!HitResult.GetActor()) return;

	if (CastWCharacter(HitResult.GetActor()))
	{
		SelectPlayerCharacter = CastWCharacter(HitResult.GetActor());
	}
	else if (Cast<AC_SMSRiding>(HitResult.GetActor()))
	{
		SelectPlayerCharacter = Cast<AC_SMSRiding>(HitResult.GetActor())->CurrentRider;
	}
	else
	{
		return;
	}


	FVector2D ScreenPartyLocation;
	GetWorld()->GetFirstPlayerController()->GetMousePosition(ScreenPartyLocation.X, ScreenPartyLocation.Y);

	AC_WorldHUD* MyHUD = CastWHUD(GetMyHUD());

	//이미 위젯이있으면파괴
	if (MyHUD->CommunityInviteWidget)
	{
		MyHUD->CommunityInviteWidget->RemoveFromParent();
		MyHUD->CommunityInviteWidget = nullptr; 
	}

	//파티초대,싸우자 위젯 생성
	MyHUD->CreateMyWidget(MyHUD->CommunityInviteWidgetClass, MyHUD->CommunityInviteWidget, ESlateVisibility::Visible);
	MyHUD->CommunityInviteWidget->SetPositionInViewport(ScreenPartyLocation);
	MyHUD->CommunityInviteWidget->TextBlock_Job->SetText(FText::FromString(ENUM_TO_STRING(EPlayerJob,SelectPlayerCharacter->CharacterInfo.Job)));
	MyHUD->CommunityInviteWidget->TextBlock_Name->SetText(FText::FromString(SelectPlayerCharacter->CharacterInfo.CharacterName));
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
}

void AC_PlayerCharacter::MultiRun_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
}

void AC_PlayerCharacter::ServerRun_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;
	MultiRun();
}

void AC_PlayerCharacter::ServerWalk_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
}

void AC_PlayerCharacter::ToggleInventory(float Value)
{
	AC_WorldHUD* MyHUD = CastWHUD(GetMyHUD());

	if (!MyHUD) return;

	// 이미 위젯이 있으면 파괴
	if (MyHUD->InventoryWidget)
	{
		MyHUD->SetInventoryWidgetVisible(false);
		MyHUD->InventoryWidget = nullptr;

		if (MyHUD->EquipmentWidget)
		{
			MyHUD->SetEquipmentWidgetVisible(false);
			MyHUD->EquipmentWidget = nullptr;
			return;
		}
		return;
	}

	MyHUD->SetInventoryWidgetVisible(true);
	MyHUD->SetEquipmentWidgetVisible(true);
}

void AC_PlayerCharacter::OnOffMouse(float Value)
{
	if (!bOnMouse)
	{
		CastWHUD(GetMyHUD())->SetInputMode(EInputMode::GameAndUI, true); // 마우스 보여주기
	}
	else
	{
		CastWHUD(GetMyHUD())->SetInputMode(EInputMode::GameOnly, false); // 마우스 숨기기	
	}

	bOnMouse = !bOnMouse;
}


void AC_PlayerCharacter::ConsumeOne(float Value)
{
	GetEquipmentComponent()->UseItem(5);
}

void AC_PlayerCharacter::ConsumeTwo(float Value)
{
	GetEquipmentComponent()->UseItem(6);
}

void AC_PlayerCharacter::OnRep_CurHeat()
{
}

void AC_PlayerCharacter::SetColor()
{
}

void AC_PlayerCharacter::NewColor(float Value)
{
}

void AC_PlayerCharacter::ButtonColor(FLinearColor Color,int32 Index)
{
}

void AC_PlayerCharacter::ServerOnShield_Implementation()
{
	bOnShield = !bOnShield;
}

void AC_PlayerCharacter::FindTarget()
{
	FVector Start = GetActorLocation()+ (GetActorForwardVector()* TRACE_TARGET_RADIUS); // 캐릭터앞에서부터
	FVector End = ThirdPersonCamera->GetComponentLocation() + ThirdPersonCamera->GetForwardVector() * TRACE_TARGET_DISTANCE; // 카메라 방향의 도착점으로

	End.Z += 800;//조금 위쪽으로 조정

	FHitResult HitResult;

	TArray<TEnumAsByte<EObjectTypeQuery>> EnemyObjectTypes;
	EnemyObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery7);

	UKismetSystemLibrary::SphereTraceSingleForObjects
	(
		this,
		Start,
		End,
		TRACE_TARGET_RADIUS,
		EnemyObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::None,
		HitResult,
		true
	);


	if (!HitResult.GetActor()) //타겟이 없을때
	{
		TargetCharacter = nullptr;
		if(CastWHUD(GetMyHUD())->PlayerWidget)
		CastWHUD(GetMyHUD())->PlayerWidget->WBPC_EnemyInfoWidget->OffTarget();
	}
	else //타겟이 있을때
	{
		TargetCharacter = Cast<AC_SMSCharacter>(HitResult.GetActor());
		if (CastWHUD(GetMyHUD())->PlayerWidget)
		CastWHUD(GetMyHUD())->PlayerWidget->WBPC_EnemyInfoWidget->OnTarget(TargetCharacter);
	}
	
	//서버에 타겟 갱신
	ServerSetTarget(TargetCharacter);
}




// 카멤라 쉐이크
void AC_PlayerCharacter::PlayCameraShake2_Implementation(TSubclassOf<UCameraShakeBase> ShakeClass)
{
	AC_WorldPlayerController* PC = Cast<AC_WorldPlayerController>(GetController());
	if (PC && PC->PlayerCameraManager && ShakeClass)
	{
		PC->PlayerCameraManager->StartCameraShake(ShakeClass);
	}

}


void AC_PlayerCharacter::ServerLoadQuest_Implementation()
{
	for (FQuestData QuestData : GetLoadQuestFromCSV(this))
	{
		if (QuestData.OwnerName != CharacterInfo.CharacterName) continue;		
		ClientAddQuest(QuestData);
	}

}

void AC_PlayerCharacter::ClientAddQuest_Implementation(FQuestData NewQuestData)
{
	AC_SMSQuest* NewQuest = nullptr;

	if (NewQuestData.QuestTitle.ToString() == TEXT("사람들을 살려줘!"))
	{
		NewQuest = NewObject<AC_SMSQuest>(this, CastWGI(GetMyGI())->QuestManager->PickUp);
	}
	else if (NewQuestData.QuestTitle.ToString() == TEXT("몬스터 학살자"))
	{
		NewQuest = NewObject<AC_SMSQuest>(this, CastWGI(GetMyGI())->QuestManager->KillMonster);
	}
	else if (NewQuestData.QuestTitle.ToString() == TEXT("출사표"))
	{
		NewQuest = NewObject<AC_SMSQuest>(this, CastWGI(GetMyGI())->QuestManager->DefeatBoss);
	}
	else
	{
		return;
	}

	NewQuest->QuestData.OwnerName = NewQuestData.OwnerName;
	NewQuest->QuestData.QuestTitle = NewQuestData.QuestTitle;
	NewQuest->QuestData.QuestState = NewQuestData.QuestState;
	NewQuest->QuestData.CurNum = NewQuestData.CurNum;


	QuestArr.Add(NewQuest);
	CastWHUD(GetMyHUD())->SetQuestingWidgetVisible(true);
	CastWHUD(GetMyHUD())->AddQuestText(NewQuest);
}

void AC_PlayerCharacter::SetCollisionActive()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}



void AC_PlayerCharacter::Client_SetViewTarget_Implementation(AActor* NewViewTarget, float BlendTime)
{
	AC_SMSPlayerController* PC = Cast<AC_SMSPlayerController>(GetController());
	if (PC && NewViewTarget)
	{
		// 현재 컨트롤러의 뷰 타겟을 저장해둠
		PreviousViewTarget = PC->GetViewTarget();

		// 시점 전환
		PC->SetViewTargetWithBlend(NewViewTarget, BlendTime);
	}
}



void AC_PlayerCharacter::Client_ResetViewTarget_Implementation(float BlendTime)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PreviousViewTarget)
	{
		// 원래 뷰 타겟으로 복귀
		PC->SetViewTargetWithBlend(PreviousViewTarget, BlendTime);
	}
}



void AC_PlayerCharacter::RemoveMappingContext_Implementation()
{

	if (!IsLocallyControlled()) return;// 서버에서는 실행 X

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		if (Subsystem && IMC_Look)
		{
			Subsystem->RemoveMappingContext(IMC_Look);
		}
	}

}

void AC_PlayerCharacter::AddMappingContext_Implementation()
{

	if (!IsLocallyControlled()) return;// 서버에서는 실행 X

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PC->GetLocalPlayer()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		if (Subsystem && IMC_Look)
		{
			Subsystem->AddMappingContext(IMC_Look, 1);
		}
	}


}

void AC_PlayerCharacter::OnInvincibility_Implementation()
{

	Absolute = true;
}

void AC_PlayerCharacter::OffInvincibility_Implementation()
{

	Absolute = false;
}

void AC_PlayerCharacter::ClientSetLoading_Implementation(bool _bOnLoading, bool _OnDungeon)
{
	if (AC_WorldPlayerController* PC = Cast<AC_WorldPlayerController>(GetController()))
	{
		if (AC_WorldHUD* WorldHUD = Cast<AC_WorldHUD>(PC->GetHUD()))
		{
			WorldHUD->SetLoading(_bOnLoading, _OnDungeon);
		}
	}
}
