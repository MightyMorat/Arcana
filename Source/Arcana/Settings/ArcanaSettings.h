// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DeveloperSettings.h"
#include "ArcanaSettings.generated.h"

/**
 * 
 */
UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Arcana Settings"))
class ARCANA_API UArcanaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	static UArcanaSettings* Get() { return GetMutableDefault<UArcanaSettings>(); }

	UPROPERTY(EditAnywhere, config, Category = "Data Tables")
	TAssetPtr<UDataTable> NeedsDataTable;
};
