#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_PlayerWidget.generated.h"

class AC_WorldHUD;
class UWC_SMSProgressBar;
class UWC_SMSButton;
class UWidgetAnimation;
class UWC_PartyWidget;
class UWC_EnemyInfoWidget;
class UWC_SettingWidget;
class UWC_ChatWidget;
class UWC_HotbarWidget;
class UWC_DeathWidget;

UCLASS()
class STRONGMETALSTONE_API UWC_PlayerWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
private:
	UWC_PlayerWidget();

	virtual void NativeConstruct() override;

public:
	// ProgressBar �ʱ�ȭ
	void InitProgressBar();

	// Ư�� ProgressBar ������Ʈ
	void UpdateProgressBar(EProgressBarType type, float percent);

	// �÷��̾� UI ������Ʈ
	void UpdateAllUI();

	// ��ȭâ�� ��ġ�� �Ʒ��� UI Visibility ����
	void SetVisibilityUnderUI(bool bOnShow);

protected:
	// ��ư �ʱ�ȭ
	void InitButtons();

	// ��ư Visibility ����
	void SetVisibilityButtons(UWC_SMSButton* button, bool bOnVisibility);

	// �޴� ��ư ������ ����
	UFUNCTION()
	void OnMenuButtonClicked();

	// ���� ��ư ������ ����
	UFUNCTION()
	void OnSettingButtonClicked();

	// �� ���� ��ư ������ ����
	UFUNCTION()
	void OnMyInfoButtonClicked();

	// �κ��丮 ��ư ������ ����
	UFUNCTION()
	void OnInvenButtonClicked();

	// ��ų ��ư ������ ����
	UFUNCTION()
	void OnSkillButtonClicked();

	// ������ ��ư ������ ����
	UFUNCTION()
	void OnExitButtonClicked();

protected:
	// �޴� ��ư
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* MenuButton;
	
	// ���� ��ư
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* SettingButton;

	// �� ���� ��ư
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* MyInfoButton;

	// �κ��丮 ��ư
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* InvenButton;

	// ��ų ��ư
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* SkillButton;

	// ������ ��ư
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* ExitButton;


	// �ִϸ��̼� ���ε�
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SlideIn;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SlideOut;

public:
	// ä�� ���� �ν��Ͻ�
	UPROPERTY()
	UWC_ChatWidget* ChatWidget;

	// ������ ���� �ν��Ͻ�
	UPROPERTY(meta = (BindWidget))
	UWC_HotbarWidget* HotbarWidget;

	// ProgressBar �ν��Ͻ�
	UPROPERTY(meta = (BindWidget))
	UWC_SMSProgressBar* ExpBar;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSProgressBar* ManaBar;

	UWC_SMSProgressBar* BossBar;

	// ��Ƽ ���� �ν��Ͻ�
	UPROPERTY(meta = (BindWidget))
	UWC_PartyWidget* WBPC_PartyWidget;

	// �� ���� ���� �ν��Ͻ�
	UPROPERTY(meta = (BindWidget))
	UWC_EnemyInfoWidget* WBPC_EnemyInfoWidget;

	// ��� ���� �ν��Ͻ�
	UPROPERTY(meta = (BindWidget))
	UWC_DeathWidget* WBPC_DeathWidget;
private:
	// �޴��� �����ִ��� Ȯ���� �� ����
	bool bOnMenuOpen = false;
};
