// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameMode.h"

#include "Buffs/ArcanaBuff.h"
#include "Buffs/ArcanaBuffData.h"
#include "Characters/ArcanaPlayerCharacter.h"
#include "Conditions//ArcanaCondition.h"
#include "Engine/World.h"
#include "Settings/ArcanaSettings.h"

AArcanaGameMode::AArcanaGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AArcanaGameMode::BeginPlay()
{
	// Add starting buffs
	for (UArcanaBuffData* BuffData : StartingBuffs)
	{
		UArcanaBuff* Buff = NewObject<UArcanaBuff>();
		Buff->BuffData = BuffData;
		Buffs.Add(Buff);
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

void AArcanaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const UArcanaSettings* ArcanaSettings = UArcanaSettings::Get();

	// Clear last frame's rate data
	for (FArcanaNeedState& NeedState : NeedStates)
	{
		NeedState.Rate = 0.0f;
	}

	// Update buffs, accumulating tags and effects
	ActiveBuffTags = UpdateBuffs(EBuffUpdateTime::Default);
	ActiveBuffTags.AppendTags(UpdateBuffs(EBuffUpdateTime::LateUpdate));

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
}

FGameplayTagContainer AArcanaGameMode::UpdateBuffs(EBuffUpdateTime UpdateTime)
{
	FGameplayTagContainer AccumulatedBuffTags;

	// Accumulate need rates from buffs
	for (UArcanaBuff* Buff : Buffs)
	{
		Buff->bIsActive = true;
		UArcanaBuffData* BuffData = Buff->BuffData;
		if (!BuffData)
		{
			Buff->bIsActive = false;
			continue;
		}

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

		AccumulatedBuffTags.AppendTags(BuffData->BuffTags);

		for (FArcanaNeedState& NeedState : NeedStates)
		{
			if (float* ModifierValue = BuffData->NeedRateModifiers.Find(NeedState.Need))
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