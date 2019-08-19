// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArcanaCondition.generated.h"

/**
 * 
 */
UCLASS(Abstract, DefaultToInstanced, EditInlineNew, AutoExpandCategories=("Condition Properties"))
class ARCANA_API UArcanaCondition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	bool bNegateCondition = false;

	bool IsConditionMet(const UObject* WorldContextObject) const { return IsConditionMet_Internal(WorldContextObject) != bNegateCondition; }

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const PURE_VIRTUAL(UArcanaCondition, return true;);
};

UCLASS()
class ARCANA_API UArcanaANDCondition final : public UArcanaCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition Properties")
	TArray<UArcanaCondition*> SubConditions;

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const override final
	{
		for (const UArcanaCondition* SubCondition : SubConditions)
		{
			if (SubCondition && !SubCondition->IsConditionMet(WorldContextObject))
			{
				return false;
			}
		}

		return true;
	}
};

UCLASS()
class ARCANA_API UArcanaORCondition final : public UArcanaCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced, Category = "Condition Properties")
	TArray<UArcanaCondition*> SubConditions;

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const override final
	{
		for (const UArcanaCondition* SubCondition : SubConditions)
		{
			if (SubCondition && SubCondition->IsConditionMet(WorldContextObject))
			{
				return true;
			}
		}

		return false;
	}
};