#pragma once

#include "CoreMinimal.h"
#include "HUD/C_SMSHUD.h"
#include "C_WorldHUD.generated.h"

UCLASS()
class STRONGMETALSTONE_API AC_WorldHUD : public AC_SMSHUD
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

public:

#pragma region Visible

	// 결과창 생성 및 삭제
	UFUNCTION()
	void SetResultWidgetVisible(bool bOnShow, FString Boss);

	// 대장장이 정보 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetBlackSmithWidgetVisible(bool bOnShow);

	// 제작자 정보 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetItemMakerWidgetVisible(bool bOnShow);

	// 플레이어 정보 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetMyInfoWidgetVisible(bool bOnShow);

	// 플레이어 설정 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetSettingWidgetVisible(bool bOnShow);

	// 인벤토리 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetInventoryWidgetVisible(bool bOnShow);

	// 장비창 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetEquipmentWidgetVisible(bool bOnShow);	
	
	// 상점 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetMerchantWidgetVisible(bool bOnShow);

	// 상점 물품 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetSellAreaWidgetVisible(bool bOnShow);

	// 상점 확정 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetShopConfirmWidgetVisible(bool bOnShow);

	// 상점 개수 결정 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetConfirmPopupWidgetVisible(bool bOnShow);

	// 버리기 개수 결정 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetRemoveWidgetVisible(bool bOnShow);

	// 아이템 옵션 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetItemOptionWidgetVisible(bool bOnShow);

	// 팝업 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetPopupWidgetVisible(bool bOnShow);

	// 아래쪽의 UI를 활성/비활성화하는 함수
	UFUNCTION()
	void SetVisibilityUnderUI(bool bOnShow);

	// 퀘스트 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetQuestWidgetVisible(bool bOnShow);
	
	// 스킬창 위젯을 생성/삭제하는 함수
	UFUNCTION()
	void SetPlayerSkillWidgetVisible(bool bOnShow);

#pragma endregion

	UFUNCTION()
	void SetQuestText(AC_SMSQuest* Quest);

	UFUNCTION()
	void SetQuestingWidgetVisible(bool bOnShow);

	UFUNCTION()
	void UpdateQuestingText(AC_SMSQuest* Quest, int32 Index);
	
#pragma region Chat

	// 채팅창을 활성화하는 함수
	void SetChatWidgetVisible(bool bOnShow);

	// 채팅창 입력을 활성화하는 함수
	void ActivateChat();

	// 채팅 메시지를 추가하는 함수
	void AddChatMessage(const FString& Message);

#pragma endregion

	// ProgressBar 초기화
	void InitProgressBar();

	// 특정 ProgressBar 업데이트
	void UpdateProgressBar(EProgressBarType type, float percent);

	// 플레이어 UI 업데이트
	void UpdateAllUI();

	// 퀘스트 추가하는 함수
	void AddQuestText(AC_SMSQuest* Quest);

	void RemoveQuestText(int32 index);

	void AddResultText(FString nickName, float Damage, unsigned int Level, EPlayerJob Job); // 결과창에 캐릭터 정보 추가

	// 상점에 데이터 전송을 위한 함수
	void SetWidgetFromData(UDA_ItemData* InItemData, EShopMode InShopMode);

	// ConfirmPopupWidget 내의 아이템 설정
	void SetItemData(UDA_ItemData* InItemData, EShopMode InShopMode);

	// ConfirmPopupWidget 내의 아이템 설정
	void SetItemData(UDA_ItemData* InItemData);

	// PopupWidget의 텍스트 설정
	void SetNoticeText(const FText& text);

	// SellAreaWidget
	void InitSellItemGrid(class UDA_ItemDataList* ItemDataList);

	// RecipeWidget
	void InitRecipeGrid(class UDA_RecipeDataList* RecipeDataList);

	// 쿨타임 UI를 위한 함수
	UFUNCTION()
	void StartCooldown(AC_SMSSkillINS* InSkillInstance);

	// 마우스 모드 전환을 위한 함수
	void SetMouseMode(bool bOnMouse);

	// 로딩 함수
	void SetLoading(bool bOnLoading,bool _OnDungeon);

public:
	// 플레이어 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_PlayerWidget* PlayerWidget;

	// 채팅 위젯 인스턴스
	UPROPERTY()
	class UWC_ChatWidget* ChatWidget;

	// 커뮤 위젯 인스턴스
	UPROPERTY()
	class UWC_CommunityWidget* CommunityInviteWidget;

	// 파티초대 위젯 인스턴스
	UPROPERTY()
	class UWC_InvitePartyWidget* InvitePartyWidget;

	// 플레이어 정보 위젯 인스턴스
	UPROPERTY()
	class UWC_MyInfoWidget* MyInfoWidget;

	// 인벤토리 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_InventoryWidget* InventoryWidget;	
	
	// 장비창 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_EquipmentWidget* EquipmentWidget;	
	
	// 상점 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_MerchantWidget* MerchantWidget;

	// 상점 물품 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_SellAreaWidget* SellAreaWidget;

	// 상점 확정 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_ShopConfirmWidget* ShopConfirmWidget;

	// 상점 개수 결정 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_ConfirmPopupWidget* ConfirmPopupWidget;

	// 버리기 개수 결정 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_RemoveWidget* RemoveWidget;

	// 아이템 옵션 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_ItemOptionWidget* ItemOptionWidget;

	// 팝업 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_PopupWidget* PopupWidget;

	// 퀘스트 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_QuestWidget* QuestWidget;

	// 퀘스트 진행 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_QuestingWidget* QuestingWidget;

	// 퀘스트 항목 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_QuestingTextWidget* QuestTextWidget;

	// 결과창 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_ResultWidget* ResultWidget;

	// 결과창 상세 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_ResultCharacterWidget* ResultCharacterWidget;

	// 로딩 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_LoadingWidget* LoadingWidget;

	// 설정 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_SettingWidget* WBPC_SettingWidget;

	// 커뮤 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CommunityInviteWidgetClass;

	// 파티초대 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> InvitePartyWidgetClass;

	// 플레이어 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	// 대장장이 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_BlackSmithWidget* BlackSmithWidget;

	// 제작자 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_ItemMakerWidget* ItemMakerWidget;

	// 스킬창 위젯 인스턴스
	UPROPERTY(BlueprintReadWrite)
	class UWC_PlayerSkillWidget* PlayerSkillWidget;

private:


	// 핫바 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HotbarWidgetClass;

	// 인벤토리 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	// 장비 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EquipmentWidgetClass;

	// 상점 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MerchantWidgetClass;

	// 상점 물품 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SellAreaWidgetClass;

	// 상점 확정 물품 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ShopConfirmWidgetClass;

	// 상점 개수 결정 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ConfirmPopupWidgetClass;

	// 버리기 개수 결정 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> RemoveWidgetClass;

	// 아이템 옵션 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemOptionWidgetClass;

	// 상점 개수 결정 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PopupWidgetClass;

	// 채팅창 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ChatWidgetClass;

	// 채팅 메시지 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ChatMessageWidgetClass;

	// 플레이어 정보 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MyInfoWidgetClass;

	// 퀘스트 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestWidgetClass;

	// 설정 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SettingWidgetClass;

	// 퀘스트 진행 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestingWidgetClass;

	// 퀘스트 항목 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestTextWidgetClass;

	// 결과창 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ResultWidgetClass;

	// 결과창 상세 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ResultCharacterWidgetClass;

	// 로딩 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	// 대장장이 상세 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> BlackSmithWidgetClass;

	// 제작자 상세 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ItemMakerWidgetClass;

	// 스킬창 위젯 클래스
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PlayerSkillWidgetClass;
};
