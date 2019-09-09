// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArcanaTime.generated.h"

UENUM(BlueprintType)
enum class EArcanaTimeOfDay : uint8
{
	Dawn,
	Day,
	Dusk,
	Night
};

USTRUCT(BlueprintType)
struct FArcanaTimeInfo
{
	GENERATED_BODY();

public:
	void InitFromTotalTimeHours(float TotalTimeHours);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 CurrentDay = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProgressThroughDay = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 CurrentHour = 0;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float ProgressThroughHour = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EArcanaTimeOfDay TimeOfDay = EArcanaTimeOfDay::Night;
};