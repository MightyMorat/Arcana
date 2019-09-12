// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actions/ArcanaAction.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Conditions/ArcanaCondition.h"
#include "InteractiveObjectComponent.generated.h"

class UArcanaActionGroup;

USTRUCT(BlueprintType)
struct FArcanaConditionalText
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UArcanaCondition*> Conditions;

	UPROPERTY(EditAnywhere)
	FText InspectText;

	bool AreConditionsMet(const UObject* WorldContextObject) const;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANA_API UInteractiveObjectComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UInteractiveObjectComponent();

	void OnHovered();
	void OnUnhovered();
	void OnSelected();
	void OnDeselected();

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<FArcanaAction> GetAvailableActions() const;

	FText GetInspectText() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	FVector GetLocatorPosition(FName LocatorId) const;

protected:
	void SetActorOutlineEnabled(bool bEnable);

	TSubclassOf<class UUserWidget> InteractOptionsWidgetClass;
	UUserWidget* InteractOptionsWidget = nullptr;

	UPROPERTY()
	class UWidgetComponent* InteractOptionsWidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UArcanaActionGroup* ActionGroup = nullptr;

	/** The first in the list with it's conditions met will be the text displayed on inspect */
	UPROPERTY(EditDefaultsOnly)
	TArray<FArcanaConditionalText> ConditionalInspectText;

	/** The inspect text to display if none of the conditional text has its conditions satisfied */
	UPROPERTY(EditDefaultsOnly)
	FText DefaultInspectText;

	bool bIsHovered = false;
	bool bIsSelected = false;
};
