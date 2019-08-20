// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ArcanaSkill.generated.h"

USTRUCT(BlueprintType)
struct FArcanaSkillState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 CurrentLevel;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ProgressToNextLevel = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ProgressRate = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UArcanaSkillDefinition* SkillDefinition = nullptr;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ARCANA_API UArcanaSkillDefinition : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int32 MaxLevel = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UDataAsset* UIData = nullptr;
};
