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
	// ProgressBar 초기화
	void InitProgressBar();

	// 특정 ProgressBar 업데이트
	void UpdateProgressBar(EProgressBarType type, float percent);

	// 플레이어 UI 업데이트
	void UpdateAllUI();

	// 대화창에 겹치는 아래쪽 UI Visibility 설정
	void SetVisibilityUnderUI(bool bOnShow);

protected:
	// 버튼 초기화
	void InitButtons();

	// 버튼 Visibility 설정
	void SetVisibilityButtons(UWC_SMSButton* button, bool bOnVisibility);

	// 메뉴 버튼 누르면 실행
	UFUNCTION()
	void OnMenuButtonClicked();

	// 설정 버튼 누르면 실행
	UFUNCTION()
	void OnSettingButtonClicked();

	// 내 정보 버튼 누르면 실행
	UFUNCTION()
	void OnMyInfoButtonClicked();

	// 인벤토리 버튼 누르면 실행
	UFUNCTION()
	void OnInvenButtonClicked();

	// 스킬 버튼 누르면 실행
	UFUNCTION()
	void OnSkillButtonClicked();

	// 나가기 버튼 누르면 실행
	UFUNCTION()
	void OnExitButtonClicked();

protected:
	// 메뉴 버튼
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* MenuButton;
	
	// 설정 버튼
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* SettingButton;

	// 내 정보 버튼
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* MyInfoButton;

	// 인벤토리 버튼
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* InvenButton;

	// 스킬 버튼
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* SkillButton;

	// 나가기 버튼
	UPROPERTY(meta = (BindWidget))
	UWC_SMSButton* ExitButton;


	// 애니메이션 바인딩
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SlideIn;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SlideOut;

public:
	// 채팅 위젯 인스턴스
	UPROPERTY()
	UWC_ChatWidget* ChatWidget;

	// 퀵슬롯 위젯 인스턴스
	UPROPERTY(meta = (BindWidget))
	UWC_HotbarWidget* HotbarWidget;

	// ProgressBar 인스턴스
	UPROPERTY(meta = (BindWidget))
	UWC_SMSProgressBar* ExpBar;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UWC_SMSProgressBar* ManaBar;

	UWC_SMSProgressBar* BossBar;

	// 파티 위젯 인스턴스
	UPROPERTY(meta = (BindWidget))
	UWC_PartyWidget* WBPC_PartyWidget;

	// 적 정보 위젯 인스턴스
	UPROPERTY(meta = (BindWidget))
	UWC_EnemyInfoWidget* WBPC_EnemyInfoWidget;

	// 사망 위젯 인스턴스
	UPROPERTY(meta = (BindWidget))
	UWC_DeathWidget* WBPC_DeathWidget;
private:
	// 메뉴가 열려있는지 확인할 불 변수
	bool bOnMenuOpen = false;
};
