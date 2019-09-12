// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ArcanaHUD.generated.h"

class UInteractiveObjectComponent;

/**
 * 
 */
UCLASS()
class ARCANA_API AArcanaHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "ArcanaHUD")
	void ShowInteractOptions(UInteractiveObjectComponent* TargetInteractiveObjectComponent);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "ArcanaHUD")
	void HideInteractOptions();

	UFUNCTION(BlueprintImplementableEvent, Category = "ArcanaHUD")
	void DisplayNotification(const FText& NotificationText);
};
