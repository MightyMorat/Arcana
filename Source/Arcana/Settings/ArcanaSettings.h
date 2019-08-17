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

	UPROPERTY(EditAnywhere, config, Category = "Needs")
	TAssetPtr<UDataTable> NeedsDataTable;

	/** The minimum need value for 'High' satisfaction */
	UPROPERTY(EditAnywhere, config, Category = "Needs")
	float HighNeedThreshold = 0.7f;

	/** The minimum need value for 'Medium' satisfaction */
	UPROPERTY(EditAnywhere, config, Category = "Needs")
	float MediumNeedThreshold = 0.3f;
};
