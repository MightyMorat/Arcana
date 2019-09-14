// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaActionEffects.h"

#include "Characters/ArcanaPlayerCharacter.h"
#include "Conditions/ArcanaCondition.h"
#include "Engine/World.h"
#include "GameMode/ArcanaGameMode.h"

void UArcanaEndActionEffect::TriggerEffect(UArcanaQueuedAction* QueuedAction) const
{
	UWorld* World = QueuedAction ? QueuedAction->GetWorld() : nullptr;
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;
	AArcanaPlayerCharacter* PlayerCharacter = GameMode ? GameMode->GetPlayerCharacter() : nullptr;
	if (PlayerCharacter)
	{
		PlayerCharacter->CancelQueuedAction(QueuedAction);
	}
}

void UArcanaConditionedEffects::TriggerEffect(UArcanaQueuedAction* QueuedAction) const
{
	for (const UArcanaCondition* Condition : Conditions)
	{
		if (Condition && !Condition->IsConditionMet(QueuedAction))
		{
			// Conditions are not met, trigger appropriate effects
			TriggerEffectList(EffectsIfFalse, QueuedAction);
			return;
		}
	}

	// Conditions are met, trigger appropriate effects
	TriggerEffectList(EffectsIfTrue, QueuedAction);
}

void UArcanaRandomEffects::TriggerEffect(UArcanaQueuedAction* QueuedAction) const
{
	if (PossibleEffectLists.Num() == 0)
		return;

	float TotalWeight = 0.0f;
	for (const FArcanaWeightedEffectList& EffectList : PossibleEffectLists)
	{
		TotalWeight += EffectList.Weight;
	}

	// If nothing has weight, just pick with equal probability
	if (TotalWeight <= 0.0f)
	{
		int32 RandomIndex = FMath::RandRange(0, PossibleEffectLists.Num() - 1);
		TriggerEffectList(PossibleEffectLists[RandomIndex].Effects, QueuedAction);
		return;
	}

	float RandomWeight = FMath::RandRange(0.0f, TotalWeight);
	float AccumulatedWeight = 0.0f;
	for (const FArcanaWeightedEffectList& EffectList : PossibleEffectLists)
	{
		AccumulatedWeight += EffectList.Weight;
		if (AccumulatedWeight >= RandomWeight)
		{
			TriggerEffectList(EffectList.Effects, QueuedAction);
			return;
		}
	}
}

void UArcanaGrantCurrencyEffect::TriggerEffect(UArcanaQueuedAction* QueuedAction) const
{
	UWorld* World = QueuedAction ? QueuedAction->GetWorld() : nullptr;
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;	
	if (GameMode)
	{
		GameMode->GrantCurrency(AmountToGrant);
	}
}