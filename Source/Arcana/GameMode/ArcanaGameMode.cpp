// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameMode.h"

#include "Buffs/ArcanaBuff.h"
#include "Buffs/ArcanaBuffData.h"
#include "Characters/ArcanaPlayerCharacter.h"
#include "Conditions//ArcanaCondition.h"
#include "Engine/World.h"
#include "FunctionLibraries/ArcanaFunctionLibrary.h"
#include "Settings/ArcanaSettings.h"

AArcanaGameMode::AArcanaGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AArcanaGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Add starting buffs
	for (UArcanaBuffData* BuffData : StartingBuffs)
	{
		UArcanaBuff* Buff = NewObject<UArcanaBuff>();
		Buff->BuffData = BuffData;
		Buffs.Add(Buff);
	}

	// Populate skill states
	SkillStates.Empty();
	UDataTable* SkillsDataTable = UArcanaSettings::Get()->SkillsDataTable.LoadSynchronous();
	if (SkillsDataTable)
	{
		const TArray<FName> SkillNames = SkillsDataTable->GetRowNames();
		for (const FName& SkillName : SkillNames)
		{
			FArcanaSkillState& SkillState = SkillStates.AddDefaulted_GetRef();
			SkillState.Skill.SkillId = SkillName;
		}
	}
}

APawn* AArcanaGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	// Also spawn the player character at the spawn transform
	if (PlayerCharacterClass)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save the player character into a map
		PlayerCharacter = GetWorld()->SpawnActor<AArcanaPlayerCharacter>(PlayerCharacterClass, SpawnTransform, SpawnInfo);
	}

	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void AArcanaGameMode::GetNeedState(FArcanaNeed Need, bool& bFound, FArcanaNeedState& OutNeedState) const
{
	bFound = false;
	OutNeedState = FArcanaNeedState();

	for (const FArcanaNeedState& NeedState : NeedStates)
	{
		if (NeedState.Need == Need)
		{
			bFound = true;
			OutNeedState = NeedState;
			return;
		}
	}
}

TArray<FArcanaNeed> AArcanaGameMode::GetActiveNeeds() const
{
	TArray<FArcanaNeed> ActiveNeeds;
	for (const FArcanaNeedState& NeedState : NeedStates)
	{
		ActiveNeeds.Add(NeedState.Need);
	}

	return ActiveNeeds;
}

void AArcanaGameMode::GetSkillState(FArcanaSkill Skill, bool& bFound, FArcanaSkillState& OutSkillState) const
{
	bFound = false;
	OutSkillState = FArcanaSkillState();

	for (const FArcanaSkillState& SkillState : SkillStates)
	{
		if (SkillState.Skill == Skill)
		{
			bFound = true;
			OutSkillState = SkillState;
			return;
		}
	}
}

TArray<FArcanaSkill> AArcanaGameMode::GetActiveSkills() const
{
	TArray<FArcanaSkill> ActiveSkills;
	for (const FArcanaSkillState& SkillState : SkillStates)
	{
		ActiveSkills.Add(SkillState.Skill);
	}

	return ActiveSkills;
}

void AArcanaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const UArcanaSettings* ArcanaSettings = UArcanaSettings::Get();

	// Clear last frame's rate data
	for (FArcanaNeedState& NeedState : NeedStates)
	{
		NeedState.Rate = 0.0f;
	}
	for (FArcanaSkillState& SkillState : SkillStates)
	{
		SkillState.ProgressRate = 0.0f;
	}

	// Update buffs, accumulating tags and effects
	ActiveBuffTags = UpdateBuffs(EBuffUpdateTime::Default);
	ActiveBuffTags.AppendTags(UpdateBuffs(EBuffUpdateTime::LateUpdate));

	// Update needs
	for (FArcanaNeedState& NeedState : NeedStates)
	{
		// Update value based on current rate
		NeedState.Value += DeltaSeconds*NeedState.Rate;
		NeedState.Value = FMath::Clamp(NeedState.Value, 0.0f, 1.0f);

		// Calculate satisfaction from value thresholds
		if (NeedState.Value >= ArcanaSettings->HighNeedThreshold)
		{
			NeedState.NeedSatisfaction = ENeedSatisfaction::High;
		}
		else if (NeedState.Value >= ArcanaSettings->MediumNeedThreshold)
		{
			NeedState.NeedSatisfaction = ENeedSatisfaction::Medium;
		}
		else
		{
			NeedState.NeedSatisfaction = ENeedSatisfaction::Low;
		}
	}

	// Update skills
	for (FArcanaSkillState& SkillState : SkillStates)
	{
		FArcanaSkillDefinition SkillDefinition;
		bool bFound = false;
		UArcanaFunctionLibrary::GetSkillDefinition(SkillState.Skill, bFound, SkillDefinition);

		if (!bFound)
			continue;

		if (SkillState.CurrentLevel == SkillDefinition.MaxLevel)
		{
			SkillState.ProgressRate = 0.0f;
			continue;
		}

		// Update value based on current rate
		SkillState.ProgressToNextLevel += DeltaSeconds * SkillState.ProgressRate;
		while (SkillState.ProgressToNextLevel >= 1.0f)
		{
			++SkillState.CurrentLevel;

			if (SkillState.CurrentLevel == SkillDefinition.MaxLevel)
			{
				SkillState.ProgressToNextLevel = 1.0f;
				break;
			}

			SkillState.ProgressToNextLevel -= 1.0f;
		}
	}
}

FGameplayTagContainer AArcanaGameMode::UpdateBuffs(EBuffUpdateTime UpdateTime)
{
	FGameplayTagContainer AccumulatedBuffTags;

	// Accumulate need rates from buffs
	for (UArcanaBuff* Buff : Buffs)
	{
		UArcanaBuffData* BuffData = Buff->BuffData;
		if (!BuffData)
		{
			Buff->bIsActive = false;
			continue;
		}

		if (BuffData->UpdateTime != UpdateTime)
			continue;

		Buff->bIsActive = true;

		for (UArcanaCondition* Condition : BuffData->OngoingConditions)
		{
			if (Condition && !Condition->IsConditionMet(this))
			{
				Buff->bIsActive = false;
				break;
			}
		}

		if (!Buff->bIsActive)
			continue;

		AccumulatedBuffTags.AppendTags(BuffData->AutogeneratedTags);
		AccumulatedBuffTags.AppendTags(BuffData->AdditionalTags);

		for (FArcanaNeedState& NeedState : NeedStates)
		{
			if (float* ModifierValue = BuffData->NeedRateModifiers.Find(NeedState.Need))
			{
				NeedState.Rate += *ModifierValue;
			}
		}

		for (FArcanaSkillState& SkillState : SkillStates)
		{
			if (float* ModifierValue = BuffData->SkillRateModifiers.Find(SkillState.Skill))
			{
				SkillState.ProgressRate += *ModifierValue;
			}
		}
	}

	return AccumulatedBuffTags;
}

TArray<UArcanaBuff*> AArcanaGameMode::GetActiveBuffs(FArcanaNeed AffectedNeed) const
{
	TArray<UArcanaBuff*> ActiveBuffs;

	for (UArcanaBuff* Buff : Buffs)
	{
		if (!Buff->bIsActive || !Buff->BuffData)
			continue;

		if (Buff->BuffData->NeedRateModifiers.Contains(AffectedNeed))
		{
			ActiveBuffs.Add(Buff);
		}
	}

	return ActiveBuffs;
}