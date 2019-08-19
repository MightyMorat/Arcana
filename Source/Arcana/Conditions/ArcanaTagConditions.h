// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaCondition.h"
#include "Classes/GameplayTagContainer.h"

#include "ArcanaTagConditions.generated.h"

UCLASS()
class ARCANA_API UArcanaBuffTagCondition final : public UArcanaCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	FGameplayTagQuery TagQuery;

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const override final;
};