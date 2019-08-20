// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ArcanaSkill.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaSkill
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName SkillId;

	FORCEINLINE bool operator==(const FArcanaSkill& OtherSkill) const { return SkillId == OtherSkill.SkillId; }
};

FORCEINLINE uint32 GetTypeHash(const FArcanaSkill& Skill)
{
	return GetTypeHash(Skill.SkillId);
}

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaSkillDefinition : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	int32 MaxLevel = 5;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	FText Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	UTexture2D* IconTexture;
};

USTRUCT(BlueprintType)
struct FArcanaSkillState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FArcanaSkill Skill;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 CurrentLevel;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ProgressToNextLevel = 0.0f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float ProgressRate = 0.0f;
};
