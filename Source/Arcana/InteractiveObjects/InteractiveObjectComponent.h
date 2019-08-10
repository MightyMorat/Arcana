// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "InteractiveObjectComponent.generated.h"


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

protected:
	void SetActorOutlineEnabled(bool bEnable);

	TSubclassOf<class UUserWidget> InteractOptionsWidgetClass;
	UUserWidget* InteractOptionsWidget = nullptr;

	UPROPERTY()
	class UWidgetComponent* InteractOptionsWidgetComponent = nullptr;

	bool bIsHovered = false;
	bool bIsSelected = false;
		
};
