// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Actions/ArcanaAction.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractiveObjectComponent.generated.h"

class UArcanaActionGroup;

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

	bool bIsHovered = false;
	bool bIsSelected = false;
};
