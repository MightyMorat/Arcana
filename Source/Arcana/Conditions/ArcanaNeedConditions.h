// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaCondition.h"
#include "Needs/ArcanaNeed.h"

#include "ArcanaNeedConditions.generated.h"

UCLASS()
class ARCANA_API UArcanaNeedSatisfactionCondition final : public UArcanaCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	FArcanaNeed Need;

	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	ENeedSatisfaction MinimumSatisfaction = ENeedSatisfaction::Low;

	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	ENeedSatisfaction MaximumSatisfaction = ENeedSatisfaction::Full;

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const override final;
};

UCLASS()
class ARCANA_API UArcanaHasNeedCondition final : public UArcanaCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	FArcanaNeed Need;

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const override final;
};

UCLASS()
class ARCANA_API UArcanaCurrencyCondition final : public UArcanaCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition Properties", meta = (ClampMin = 0))
	int32 MinimumCurrency = 0;

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const override final;
};