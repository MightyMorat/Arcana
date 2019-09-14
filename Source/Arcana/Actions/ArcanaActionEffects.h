// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanaActionEvent.h"
#include "ArcanaActionEffects.generated.h"

class UArcanaCondition;

UCLASS()
class ARCANA_API UArcanaEndActionEffect : public UArcanaActionEffect
{
	GENERATED_BODY()

public:
	virtual void TriggerEffect(UArcanaQueuedAction* QueuedAction) const override;
};

UCLASS()
class ARCANA_API UArcanaConditionedEffects : public UArcanaActionEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Effect Properties")
	TArray<UArcanaCondition*> Conditions;

	UPROPERTY(EditAnywhere, Instanced, Category = "Effect Properties")
	TArray<UArcanaActionEffect*> EffectsIfTrue;

	UPROPERTY(EditAnywhere, Instanced, Category = "Effect Properties")
	TArray<UArcanaActionEffect*> EffectsIfFalse;

	virtual void TriggerEffect(UArcanaQueuedAction* QueuedAction) const override;
};

USTRUCT()
struct FArcanaWeightedEffectList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Effect Properties", meta = (ClampMin = 0.0f))
	float Weight = 1.0f;

	UPROPERTY(EditAnywhere, Instanced, Category = "Effect Properties")
	TArray<UArcanaActionEffect*> Effects;
};

UCLASS()
class ARCANA_API UArcanaRandomEffects : public UArcanaActionEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Effect Properties")
	TArray<FArcanaWeightedEffectList> PossibleEffectLists;

	virtual void TriggerEffect(UArcanaQueuedAction* QueuedAction) const override;
};

UCLASS()
class ARCANA_API UArcanaGrantCurrencyEffect : public UArcanaActionEffect
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Effect Properties")
	int32 AmountToGrant = 1;

	virtual void TriggerEffect(UArcanaQueuedAction* QueuedAction) const override;
};