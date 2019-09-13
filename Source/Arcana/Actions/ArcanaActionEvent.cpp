// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaActionEvent.h"

#include "Characters/ArcanaPlayerCharacter.h"
#include "Conditions/ArcanaCondition.h"

/*static*/ void UArcanaActionEffect::TriggerEffectList(const TArray<UArcanaActionEffect*>& EffectList, UArcanaQueuedAction* QueuedAction)
{
	for (const UArcanaActionEffect* Effect : EffectList)
	{
		if (Effect)
		{
			Effect->TriggerEffect(QueuedAction);
		}
	}
}

bool UArcanaActionEvent::AreConditionsMet(const UArcanaQueuedAction* QueuedAction) const
{
	for (const UArcanaCondition* Condition : Conditions)
	{
		if (Condition && !Condition->IsConditionMet(QueuedAction))
		{
			return false;
		}
	}

	return true;
}

void UArcanaActionEvent::TriggerEffects(UArcanaQueuedAction* QueuedAction) const
{
	UArcanaActionEffect::TriggerEffectList(Effects, QueuedAction);
}