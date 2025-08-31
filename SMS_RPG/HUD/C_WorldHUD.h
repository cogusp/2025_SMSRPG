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

	// ���â ���� �� ����
	UFUNCTION()
	void SetResultWidgetVisible(bool bOnShow, FString Boss);

	// �������� ���� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetBlackSmithWidgetVisible(bool bOnShow);

	// ������ ���� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetItemMakerWidgetVisible(bool bOnShow);

	// �÷��̾� ���� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetMyInfoWidgetVisible(bool bOnShow);

	// �÷��̾� ���� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetSettingWidgetVisible(bool bOnShow);

	// �κ��丮 ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetInventoryWidgetVisible(bool bOnShow);

	// ���â ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetEquipmentWidgetVisible(bool bOnShow);	
	
	// ���� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetMerchantWidgetVisible(bool bOnShow);

	// ���� ��ǰ ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetSellAreaWidgetVisible(bool bOnShow);

	// ���� Ȯ�� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetShopConfirmWidgetVisible(bool bOnShow);

	// ���� ���� ���� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetConfirmPopupWidgetVisible(bool bOnShow);

	// ������ ���� ���� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetRemoveWidgetVisible(bool bOnShow);

	// ������ �ɼ� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetItemOptionWidgetVisible(bool bOnShow);

	// �˾� ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetPopupWidgetVisible(bool bOnShow);

	// �Ʒ����� UI�� Ȱ��/��Ȱ��ȭ�ϴ� �Լ�
	UFUNCTION()
	void SetVisibilityUnderUI(bool bOnShow);

	// ����Ʈ ������ ����/�����ϴ� �Լ�
	UFUNCTION()
	void SetQuestWidgetVisible(bool bOnShow);
	
	// ��ųâ ������ ����/�����ϴ� �Լ�
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

	// ä��â�� Ȱ��ȭ�ϴ� �Լ�
	void SetChatWidgetVisible(bool bOnShow);

	// ä��â �Է��� Ȱ��ȭ�ϴ� �Լ�
	void ActivateChat();

	// ä�� �޽����� �߰��ϴ� �Լ�
	void AddChatMessage(const FString& Message);

#pragma endregion

	// ProgressBar �ʱ�ȭ
	void InitProgressBar();

	// Ư�� ProgressBar ������Ʈ
	void UpdateProgressBar(EProgressBarType type, float percent);

	// �÷��̾� UI ������Ʈ
	void UpdateAllUI();

	// ����Ʈ �߰��ϴ� �Լ�
	void AddQuestText(AC_SMSQuest* Quest);

	void RemoveQuestText(int32 index);

	void AddResultText(FString nickName, float Damage, unsigned int Level, EPlayerJob Job); // ���â�� ĳ���� ���� �߰�

	// ������ ������ ������ ���� �Լ�
	void SetWidgetFromData(UDA_ItemData* InItemData, EShopMode InShopMode);

	// ConfirmPopupWidget ���� ������ ����
	void SetItemData(UDA_ItemData* InItemData, EShopMode InShopMode);

	// ConfirmPopupWidget ���� ������ ����
	void SetItemData(UDA_ItemData* InItemData);

	// PopupWidget�� �ؽ�Ʈ ����
	void SetNoticeText(const FText& text);

	// SellAreaWidget
	void InitSellItemGrid(class UDA_ItemDataList* ItemDataList);

	// RecipeWidget
	void InitRecipeGrid(class UDA_RecipeDataList* RecipeDataList);

	// ��Ÿ�� UI�� ���� �Լ�
	UFUNCTION()
	void StartCooldown(AC_SMSSkillINS* InSkillInstance);

	// ���콺 ��� ��ȯ�� ���� �Լ�
	void SetMouseMode(bool bOnMouse);

	// �ε� �Լ�
	void SetLoading(bool bOnLoading,bool _OnDungeon);

public:
	// �÷��̾� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_PlayerWidget* PlayerWidget;

	// ä�� ���� �ν��Ͻ�
	UPROPERTY()
	class UWC_ChatWidget* ChatWidget;

	// Ŀ�� ���� �ν��Ͻ�
	UPROPERTY()
	class UWC_CommunityWidget* CommunityInviteWidget;

	// ��Ƽ�ʴ� ���� �ν��Ͻ�
	UPROPERTY()
	class UWC_InvitePartyWidget* InvitePartyWidget;

	// �÷��̾� ���� ���� �ν��Ͻ�
	UPROPERTY()
	class UWC_MyInfoWidget* MyInfoWidget;

	// �κ��丮 ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_InventoryWidget* InventoryWidget;	
	
	// ���â ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_EquipmentWidget* EquipmentWidget;	
	
	// ���� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_MerchantWidget* MerchantWidget;

	// ���� ��ǰ ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_SellAreaWidget* SellAreaWidget;

	// ���� Ȯ�� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_ShopConfirmWidget* ShopConfirmWidget;

	// ���� ���� ���� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_ConfirmPopupWidget* ConfirmPopupWidget;

	// ������ ���� ���� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_RemoveWidget* RemoveWidget;

	// ������ �ɼ� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_ItemOptionWidget* ItemOptionWidget;

	// �˾� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_PopupWidget* PopupWidget;

	// ����Ʈ ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_QuestWidget* QuestWidget;

	// ����Ʈ ���� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_QuestingWidget* QuestingWidget;

	// ����Ʈ �׸� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_QuestingTextWidget* QuestTextWidget;

	// ���â ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_ResultWidget* ResultWidget;

	// ���â �� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_ResultCharacterWidget* ResultCharacterWidget;

	// �ε� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_LoadingWidget* LoadingWidget;

	// ���� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_SettingWidget* WBPC_SettingWidget;

	// Ŀ�� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> CommunityInviteWidgetClass;

	// ��Ƽ�ʴ� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> InvitePartyWidgetClass;

	// �÷��̾� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PlayerWidgetClass;

	// �������� ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_BlackSmithWidget* BlackSmithWidget;

	// ������ ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_ItemMakerWidget* ItemMakerWidget;

	// ��ųâ ���� �ν��Ͻ�
	UPROPERTY(BlueprintReadWrite)
	class UWC_PlayerSkillWidget* PlayerSkillWidget;

private:


	// �ֹ� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> HotbarWidgetClass;

	// �κ��丮 ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> InventoryWidgetClass;

	// ��� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> EquipmentWidgetClass;

	// ���� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MerchantWidgetClass;

	// ���� ��ǰ ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SellAreaWidgetClass;

	// ���� Ȯ�� ��ǰ ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ShopConfirmWidgetClass;

	// ���� ���� ���� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ConfirmPopupWidgetClass;

	// ������ ���� ���� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> RemoveWidgetClass;

	// ������ �ɼ� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ItemOptionWidgetClass;

	// ���� ���� ���� ���� Ŭ����
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PopupWidgetClass;

	// ä��â ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ChatWidgetClass;

	// ä�� �޽��� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ChatMessageWidgetClass;

	// �÷��̾� ���� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MyInfoWidgetClass;

	// ����Ʈ ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestWidgetClass;

	// ���� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> SettingWidgetClass;

	// ����Ʈ ���� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestingWidgetClass;

	// ����Ʈ �׸� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> QuestTextWidgetClass;

	// ���â ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ResultWidgetClass;

	// ���â �� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ResultCharacterWidgetClass;

	// �ε� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	// �������� �� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> BlackSmithWidgetClass;

	// ������ �� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ItemMakerWidgetClass;

	// ��ųâ ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PlayerSkillWidgetClass;
};
