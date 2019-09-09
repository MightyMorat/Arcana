// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaFunctionLibrary.h"

#include "Classes/Engine/Engine.h"
#include "Classes/Engine/World.h"
#include "Classes/GameFramework/WorldSettings.h"
#include "GameMode/ArcanaGameInstance.h"
#include "Settings/ArcanaSettings.h"

void UArcanaFunctionLibrary::GetNeedDefinition(FArcanaNeed Need, bool& bFound, FArcanaNeedDefinition& NeedDefinition)
{
	bFound = false;
	NeedDefinition = FArcanaNeedDefinition();

	UDataTable* NeedsDataTable = UArcanaSettings::Get()->NeedsDataTable.LoadSynchronous();
	if (NeedsDataTable)
	{
		FArcanaNeedDefinition* Definition = NeedsDataTable->FindRow<FArcanaNeedDefinition>(Need.NeedId, FString(""), false);
		if (Definition)
		{
			bFound = true;
			NeedDefinition = *Definition;
		}
	}
}

void UArcanaFunctionLibrary::GetSkillDefinition(FArcanaSkill Skill, bool& bFound, FArcanaSkillDefinition& SkillDefinition)
{
	bFound = false;
	SkillDefinition = FArcanaSkillDefinition();

	UDataTable* SkillsDataTable = UArcanaSettings::Get()->SkillsDataTable.LoadSynchronous();
	if (SkillsDataTable)
	{
		FArcanaSkillDefinition* Definition = SkillsDataTable->FindRow<FArcanaSkillDefinition>(Skill.SkillId, FString(""), false);
		if (Definition)
		{
			bFound = true;
			SkillDefinition = *Definition;
		}
	}
}

float UArcanaFunctionLibrary::GetRealDeltaSeconds(const UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
		return 0.0f;

	float DeltaSeconds = World->GetDeltaSeconds();
	const float TimeDilation = World->GetWorldSettings()->TimeDilation;
	return TimeDilation > 0.0f ? DeltaSeconds / TimeDilation : DeltaSeconds;
}

float UArcanaFunctionLibrary::GetDeltaGameHours(float DeltaSeconds)
{
	const float GameHourRealTimeSeconds = FMath::Max(UArcanaSettings::Get()->GameHourRealTimeSeconds, 1.0f);
	return DeltaSeconds / GameHourRealTimeSeconds;
}