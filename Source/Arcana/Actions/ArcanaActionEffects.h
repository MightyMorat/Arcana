// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanaActionEvent.h"
#include "ArcanaActionEffects.generated.h"

UCLASS()
class ARCANA_API UArcanaEndActionEffect : public UArcanaActionEffect
{
	GENERATED_BODY()

public:
	virtual void TriggerEffect(UArcanaQueuedAction* QueuedAction) const override;
};