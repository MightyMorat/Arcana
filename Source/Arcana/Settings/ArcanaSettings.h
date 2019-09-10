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

	/** The hour of the day when day becomes dusk (24 hour clock) */
	UPROPERTY(EditAnywhere, config, Category = "UI")
	TArray<TSoftObjectPtr<UTexture2D>> LoadingScreenTextures;

	UPROPERTY(EditAnywhere, config, Category = "Needs")
	TAssetPtr<UDataTable> NeedsDataTable;

	/** The minimum need value for 'High' satisfaction */
	UPROPERTY(EditAnywhere, config, Category = "Needs")
	float HighNeedThreshold = 0.7f;

	/** The minimum need value for 'Medium' satisfaction */
	UPROPERTY(EditAnywhere, config, Category = "Needs")
	float MediumNeedThreshold = 0.3f;

	UPROPERTY(EditAnywhere, config, Category = "Skills")
	TAssetPtr<UDataTable> SkillsDataTable;

	/** The number of seconds that must pass in real time for 1 hour to pass in game (at 1x speed) */
	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (ClampMin = 1.0f))
	float GameHourRealTimeSeconds = 30.0f;

	/** The hour of the day when dusk becomes night (24 hour clock) */
	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (ClampMin = 12, ClampMax = 23))
	int32 NightStartHour = 21;

	/** The hour of the day when night becomes dawn (24 hour clock) */
	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (ClampMin = 0, ClampMax = 11))
	int32 NightEndHour = 6;

	/** The hour of the day when dawn becomes day (24 hour clock) */
	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (ClampMin = 0, ClampMax = 11))
	int32 DayStartHour = 8;

	/** The hour of the day when day becomes dusk (24 hour clock) */
	UPROPERTY(EditAnywhere, config, Category = "Time", meta = (ClampMin = 12, ClampMax = 23))
	int32 DayEndHour = 19;
};
