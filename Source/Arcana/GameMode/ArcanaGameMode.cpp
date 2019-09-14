// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameMode.h"

#include "Actions/ArcanaAction.h"
#include "ArcanaGameInstance.h"
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

	GameInstance = GetGameInstance<UArcanaGameInstance>();
}

APawn* AArcanaGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	// Also spawn the player character at the spawn transform
	if (PlayerCharacterClass)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save the player character into a map
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		PlayerCharacter = GetWorld()->SpawnActor<AArcanaPlayerCharacter>(PlayerCharacterClass, SpawnTransform, SpawnInfo);
	}

	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void AArcanaGameMode::GetNeedState(FArcanaNeed Need, bool& bFound, FArcanaNeedState& OutNeedState) const
{
	bFound = false;
	OutNeedState = FArcanaNeedState();

	if (GameInstance)
	{
		for (const FArcanaNeedState& NeedState : GameInstance->NeedStates)
		{
			if (NeedState.Need == Need)
			{
				bFound = true;
				OutNeedState = NeedState;
				return;
			}
		}
	}
}

TArray<FArcanaNeed> AArcanaGameMode::GetActiveNeeds() const
{
	TArray<FArcanaNeed> ActiveNeeds;

	if (GameInstance)
	{
		for (const FArcanaNeedState& NeedState : GameInstance->NeedStates)
		{
			ActiveNeeds.Add(NeedState.Need);
		}
	}

	return ActiveNeeds;
}

void AArcanaGameMode::GetSkillState(FArcanaSkill Skill, bool& bFound, FArcanaSkillState& OutSkillState) const
{
	bFound = false;
	OutSkillState = FArcanaSkillState();

	if (GameInstance)
	{
		for (const FArcanaSkillState& SkillState : GameInstance->SkillStates)
		{
			if (SkillState.Skill == Skill)
			{
				bFound = true;
				OutSkillState = SkillState;
				return;
			}
		}
	}
}

TArray<FArcanaSkill> AArcanaGameMode::GetActiveSkills() const
{
	TArray<FArcanaSkill> ActiveSkills;

	if (GameInstance)
	{
		for (const FArcanaSkillState& SkillState : GameInstance->SkillStates)
		{
			ActiveSkills.Add(SkillState.Skill);
		}
	}

	return ActiveSkills;
}

void AArcanaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float DeltaGameHours = UArcanaFunctionLibrary::GetDeltaGameHours(DeltaSeconds);

	if (!GameInstance)
		return;

	GameInstance->TotalTimeHours += DeltaGameHours;

	const UArcanaSettings* ArcanaSettings = UArcanaSettings::Get();

	// Get skill rate from current action
	const UArcanaActionData* InProgressActionData = PlayerCharacter ? PlayerCharacter->GetInProgressActionData() : nullptr;
	for (FArcanaSkillState& SkillState : GameInstance->SkillStates)
	{
		if (InProgressActionData && InProgressActionData->AffectedSkill == SkillState.Skill)
		{
			SkillState.ProgressRate = InProgressActionData->SkillGainRate;
		}
		else
		{
			SkillState.ProgressRate = 0.0f;
		}
	}

	// Clear last frame's rate data
	for (FArcanaNeedState& NeedState : GameInstance->NeedStates)
	{
		NeedState.Rate = 0.0f;
	}

	// Update buffs, accumulating tags and effects
	ActiveBuffTags = UpdateBuffs(EBuffUpdateTime::Default);
	ActiveBuffTags.AppendTags(UpdateBuffs(EBuffUpdateTime::LateUpdate));

	// Update needs
	for (FArcanaNeedState& NeedState : GameInstance->NeedStates)
	{
		// Clamp rate at the boundaries
		if (NeedState.Value == 0.0f)
		{
			NeedState.Rate = FMath::Max(0.0f, NeedState.Rate);
		}
		else if (NeedState.Value == 1.0f)
		{
			NeedState.Rate = FMath::Min(0.0f, NeedState.Rate);
		}

		// Update value based on current rate
		NeedState.Value += DeltaGameHours*NeedState.Rate;
		NeedState.Value = FMath::Clamp(NeedState.Value, 0.0f, 1.0f);

		// Calculate satisfaction from value thresholds
		if (NeedState.Value >= 1.0f)
		{
			NeedState.NeedSatisfaction = ENeedSatisfaction::Full;
		}
		else if (NeedState.Value >= ArcanaSettings->HighNeedThreshold)
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
	for (FArcanaSkillState& SkillState : GameInstance->SkillStates)
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
		SkillState.ProgressToNextLevel += DeltaGameHours * SkillState.ProgressRate;
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

	if (!GameInstance)
		return AccumulatedBuffTags;

	// Accumulate need rates from buffs
	for (UArcanaBuff* Buff : GameInstance->Buffs)
	{
		const UArcanaBuffData* BuffData = Buff->BuffData;
		if (!BuffData)
		{
			Buff->bIsActive = false;
			continue;
		}

		if (BuffData->UpdateTime != UpdateTime)
			continue;

		Buff->bIsActive = true;

		for (const UArcanaCondition* Condition : BuffData->OngoingConditions)
		{
			if (Condition && !Condition->IsConditionMet(Buff->ContextObject))
			{
				Buff->bIsActive = false;
				break;
			}
		}

		if (!Buff->bIsActive)
			continue;

		AccumulatedBuffTags.AppendTags(BuffData->AutogeneratedTags);
		AccumulatedBuffTags.AppendTags(BuffData->AdditionalTags);

		for (FArcanaNeedState& NeedState : GameInstance->NeedStates)
		{
			if (const float* ModifierValue = BuffData->NeedRateModifiers.Find(NeedState.Need))
			{
				NeedState.Rate += *ModifierValue;
			}
		}
	}

	return AccumulatedBuffTags;
}

TArray<UArcanaBuff*> AArcanaGameMode::GetActiveBuffs(FArcanaNeed AffectedNeed) const
{
	TArray<UArcanaBuff*> ActiveBuffs;

	if (GameInstance)
	{
		for (UArcanaBuff* Buff : GameInstance->Buffs)
		{
			if (!Buff->bIsActive || !Buff->BuffData)
				continue;

			if (Buff->BuffData->NeedRateModifiers.Contains(AffectedNeed))
			{
				ActiveBuffs.Add(Buff);
			}
		}
	}

	return ActiveBuffs;
}

UArcanaBuff* AArcanaGameMode::ApplyBuff(const UArcanaBuffData* BuffData, UObject* ContextObject, bool bIsTransient)
{
	if (!GameInstance)
		return nullptr;

	for (const UArcanaCondition* Condition : BuffData->ApplicationConditions)
	{
		if (Condition && !Condition->IsConditionMet(ContextObject))
		{
			return nullptr;
		}
	}

	UArcanaBuff* Buff = NewObject<UArcanaBuff>();
	Buff->BuffData = BuffData;
	Buff->ContextObject = ContextObject;
	Buff->bIsTransient = bIsTransient;
	GameInstance->Buffs.Add(Buff);

	return Buff;
}

int32 AArcanaGameMode::GetCurrency() const
{
	if (!GameInstance)
		return 0;

	return GameInstance->Currency;
}

FArcanaTimeInfo AArcanaGameMode::GetTimeInfo() const
{
	FArcanaTimeInfo TimeInfo;

	if (GameInstance)
	{
		TimeInfo.InitFromTotalTimeHours(GameInstance->TotalTimeHours);
	}

	return TimeInfo;
}

void AArcanaGameMode::RemoveBuff(UArcanaBuff* Buff)
{
	if (!GameInstance)
		return;

	GameInstance->Buffs.Remove(Buff);
}

/** Console commands */
void AArcanaGameMode::SetNeedValue(FName Need, float Value)
{
	if (!GameInstance)
		return;

	for (FArcanaNeedState& NeedState : GameInstance->NeedStates)
	{
		if (NeedState.Need.NeedId == Need)
		{
			NeedState.Value = FMath::Clamp(Value, 0.0f, 1.0f);
			return;
		}
	}
}

void AArcanaGameMode::SetSkillLevel(FName Skill, int32 Level)
{
	if (!GameInstance)
		return;

	for (FArcanaSkillState& SkillState : GameInstance->SkillStates)
	{
		if (SkillState.Skill.SkillId == Skill)
		{
			FArcanaSkillDefinition SkillDefinition;
			bool bFound = false;
			UArcanaFunctionLibrary::GetSkillDefinition(SkillState.Skill, bFound, SkillDefinition);
			if (bFound)
			{
				SkillState.CurrentLevel = FMath::Clamp(Level, 0, SkillDefinition.MaxLevel);
				SkillState.ProgressToNextLevel = 0.0f;
			}

			return;
		}
	}
}