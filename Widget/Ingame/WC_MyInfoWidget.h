#pragma once

#include "CoreMinimal.h"
#include "Widget/WC_SMSUserWidget.h"
#include "WC_MyInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class STRONGMETALSTONE_API UWC_MyInfoWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()
	
private:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void OnCloseButtonClicked();

	// 플레이어 정보 세팅
	void SetPlayerInfo();

public:
	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;	// 나가기 버튼

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Name;				// 이름 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Level;				// 레벨 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Job;				// 직업 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Hp;				// HP 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Mp;				// MP 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Damage;			// 공격력 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Defense;			// 방어력 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Speed;				// 이동 속도 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_CurExp;			// 현재 경험치 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_MaxExp;			// 최대 경험치 텍스트 박스

	UPROPERTY(meta = (BindWidget))
	class UWC_SMSProgressBar* PlayerExpBar;			// 플레이어 경험치 바

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* OnSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* OffSound;
};
