// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "Needs/ArcanaNeed.h"
#include "Skills/ArcanaSkill.h"

#include "ArcanaGameInstance.generated.h"

class UArcanaBuff;
class UArcanaBuffData;

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaStartingScenario : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FArcanaNeed> StartingNeeds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<const UArcanaBuffData*> StartingBuffs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FArcanaSkill, int32> StartingSkillLevels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StartingCurrency = 0;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ARCANA_API UArcanaGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class AArcanaGameMode;

	UFUNCTION(BlueprintCallable)
	void LoadScenario(const FArcanaStartingScenario& Scenario);

private:
	TArray<FArcanaNeedState> NeedStates;
	TArray<FArcanaSkillState> SkillStates;
	int32 Currency = 0;

	UPROPERTY()
	TArray<UArcanaBuff*> Buffs;
};
