// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaFunctionLibrary.h"

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