// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaCondition.h"
#include "Skills/ArcanaSkill.h"

#include "ArcanaSkillConditions.generated.h"

UCLASS()
class ARCANA_API UArcanaSkillLevelCondition final : public UArcanaCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	FArcanaSkill Skill;

	UPROPERTY(EditAnywhere, Category = "Condition Properties")
	int32 MinimumLevel = 0;

private:
	virtual bool IsConditionMet_Internal(const UObject* WorldContextObject) const override final;
};