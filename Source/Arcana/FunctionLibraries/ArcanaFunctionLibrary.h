// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Needs/ArcanaNeed.h"
#include "Skills/ArcanaSkill.h"

#include "ArcanaFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCANA_API UArcanaFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Needs")
	static void GetNeedDefinition(FArcanaNeed Need, bool& bFound, FArcanaNeedDefinition& NeedDefinition);

	UFUNCTION(BlueprintCallable, Category = "Skills")
	static void GetSkillDefinition(FArcanaSkill Skill, bool& bFound, FArcanaSkillDefinition& SkillDefinition);

	UFUNCTION(BlueprintCallable, Category = "Time", meta = (WorldContext = "WorldContextObject"))
	static float GetRealDeltaSeconds(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Time")
	static float GetDeltaGameHours(float DeltaSeconds);
};
