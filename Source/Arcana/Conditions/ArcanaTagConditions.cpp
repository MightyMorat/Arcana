// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaTagConditions.h"

#include "Engine/World.h"
#include "GameMode/ArcanaGameMode.h"

bool UArcanaBuffTagCondition::IsConditionMet_Internal(const UObject* WorldContextObject) const
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;

	if (GameMode)
	{
		return GameMode->GetActiveBuffTags().MatchesQuery(TagQuery);
	}

	return false;
}