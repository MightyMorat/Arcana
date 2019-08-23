// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaSkillConditions.h"

#include "Engine/World.h"
#include "GameMode/ArcanaGameMode.h"

bool UArcanaSkillLevelCondition::IsConditionMet_Internal(const UObject* WorldContextObject) const
{
	UWorld* World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;

	if (GameMode)
	{
		bool bFound;
		FArcanaSkillState SkillState;
		GameMode->GetSkillState(Skill, bFound, SkillState);

		if (bFound)
		{
			if (MinimumLevel == 0)
			{
				return SkillState.CurrentLevel > 0 || SkillState.ProgressRate > 0.0f;
			}

			return SkillState.CurrentLevel >= MinimumLevel;
		}
	}

	return false;
}