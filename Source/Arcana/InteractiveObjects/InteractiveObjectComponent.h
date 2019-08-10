// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractiveObjectComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANA_API UInteractiveObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	void OnHovered();
	void OnUnhovered();
	void OnSelected();
	void OnDeselected();

protected:
	void SetActorOutlineEnabled(bool bEnable);
	bool bIsHovered = false;
	bool bIsSelected = false;
		
};
