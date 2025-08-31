#pragma once

#include "Widget/WC_SMSUserWidget.h"
#include "CoreMinimal.h"
#include "WC_ChatWidget.generated.h"

UCLASS()
class STRONGMETALSTONE_API UWC_ChatWidget : public UWC_SMSUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// Enter Ű �Է� �� ä��â�� Ȱ��ȭ�ϱ� ���� �Լ�
	UFUNCTION()
	void ActivateChat();

	// ä��â�� ���̰� �����ϴ� �Լ�
	UFUNCTION()
	void SetChatWidgetVisibility(bool bOnShow);

	// ä��â�� �޽����� �߰��ϴ� �Լ�
	UFUNCTION()
	void AddChatMessage(UWC_ChatMessageWidget* Message);

protected:
	// Enter Ű �Է� �� ä�� ����
	UFUNCTION()
	void OnTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

public:
	// ä�� �Է��� ���� �ؽ�Ʈ
	UPROPERTY(meta = (BindWidget))
	class UEditableText* ChatText;

	// ä�� ����� ���� ��ũ�� �ڽ�
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatScrollBox;

	// ��ũ�� �ڽ��� ���
	UPROPERTY(meta = (BindeWidget))
	class UImage* Background;

	// ä�� �Է� �ؽ�Ʈ�� ���
	UPROPERTY(meta = (BindWidget))
	class UBorder* BorderChatText;
};
