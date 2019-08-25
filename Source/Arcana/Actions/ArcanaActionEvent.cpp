// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaActionEvent.h"

#include "Characters/ArcanaPlayerCharacter.h"
#include "Conditions/ArcanaCondition.h"

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
	for (const UArcanaActionEffect* Effect : Effects)
	{
		if (Effect)
		{
			Effect->TriggerEffect(QueuedAction);
		}
	}
}