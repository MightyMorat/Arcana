// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaNeedConditions.h"

#include "Engine/World.h"
#include "GameMode/ArcanaGameMode.h"

bool UArcanaNeedSatisfactionCondition::IsConditionMet_Internal(const UObject* WorldContextObject) const
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;

	if (GameMode)
	{
		bool bFound;
		FArcanaNeedState NeedState;
		GameMode->GetNeedState(Need, bFound, NeedState);

		if (bFound)
		{
			return NeedState.NeedSatisfaction >= MinimumSatisfaction && NeedState.NeedSatisfaction <= MaximumSatisfaction;
		}
	}

	return false;
}

bool UArcanaHasNeedCondition::IsConditionMet_Internal(const UObject* WorldContextObject) const
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;

	if (GameMode)
	{
		bool bFound;
		FArcanaNeedState NeedState;
		GameMode->GetNeedState(Need, bFound, NeedState);

		return bFound;
	}

	return false;
}