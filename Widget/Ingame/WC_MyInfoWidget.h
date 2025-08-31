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

	// �÷��̾� ���� ����
	void SetPlayerInfo();

public:
	UPROPERTY(meta = (BindWidget))
	class UWC_SMSButton* CloseButton;	// ������ ��ư

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Name;				// �̸� �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Level;				// ���� �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Job;				// ���� �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Hp;				// HP �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Mp;				// MP �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Damage;			// ���ݷ� �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Defense;			// ���� �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Speed;				// �̵� �ӵ� �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_CurExp;			// ���� ����ġ �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_MaxExp;			// �ִ� ����ġ �ؽ�Ʈ �ڽ�

	UPROPERTY(meta = (BindWidget))
	class UWC_SMSProgressBar* PlayerExpBar;			// �÷��̾� ����ġ ��

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* OnSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* OffSound;
};
