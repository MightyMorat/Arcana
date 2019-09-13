// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanaActionEvent.generated.h"

class UArcanaCondition;
class UArcanaQueuedAction;

UENUM()
enum class EArcanaActionEventTriggerType : uint8
{
	OnActionStart,
	OnConditionsMet,
	OnActionDurationComplete
};

UCLASS(Abstract, DefaultToInstanced, EditInlineNew, AutoExpandCategories = ("Effect Properties"))
class ARCANA_API UArcanaActionEffect : public UObject
{
	GENERATED_BODY()

public:
	virtual void TriggerEffect(UArcanaQueuedAction* QueuedAction) const PURE_VIRTUAL(UArcanaActionEffect, return;);

	static void TriggerEffectList(const TArray<UArcanaActionEffect*>& EffectList, UArcanaQueuedAction* QueuedAction);
};

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew, AutoExpandCategories=("Event Properties"))
class ARCANA_API UArcanaActionEvent : public UObject
{
	GENERATED_BODY()

public:
	EArcanaActionEventTriggerType GetTriggerType() const { return TriggerType; }
	bool AreConditionsMet(const UArcanaQueuedAction* QueuedAction) const;
	virtual void TriggerEffects(UArcanaQueuedAction* QueuedAction) const;	

protected:
	UPROPERTY(EditAnywhere, Category = "Event Properties")
	EArcanaActionEventTriggerType TriggerType = EArcanaActionEventTriggerType::OnActionStart;

	UPROPERTY(EditAnywhere, Instanced, Category = "Event Properties")
	TArray<UArcanaCondition*> Conditions;

	UPROPERTY(EditAnywhere, Instanced, Category = "Event Properties")
	TArray<UArcanaActionEffect*> Effects;
};