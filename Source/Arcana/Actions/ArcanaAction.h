// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/DataAsset.h"
#include "Skills/ArcanaSkill.h"

#include "ArcanaAction.generated.h"

class UArcanaActionEvent;
class UArcanaBuffData;
class UArcanaCondition;

UCLASS(BlueprintType)
class ARCANA_API UArcanaActionData : public UDataAsset
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual bool CanEditChange(const UProperty* InProperty) const override;
#endif

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (InlineEditConditionToggle))
	bool bHasMaxDuration = false;

	/** In game hours */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 1.0f, EditCondition="bHasMaxDuration"))
	float MaxDuration = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<UArcanaBuffData*> OngoingBuffs;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FArcanaSkill AffectedSkill;

	/** Per game hour */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (ClampMin = 0.0f))
	float SkillGainRate = 0.1f;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<UArcanaActionEvent*> ActionEvents;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	UTexture2D* IconTexture;
};


USTRUCT(BlueprintType)
struct FArcanaActionCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UArcanaCondition* Condition;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bHideOnFailure = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (EditCondition = "!bHideOnFailure"))
	FText FailureNameOverride;
};

USTRUCT(BlueprintType)
struct FArcanaAction
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UArcanaActionData* ActionData = nullptr;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText ActionName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsEnabled = true;

	UPROPERTY(EditAnywhere)
	TArray<FArcanaActionCondition> Conditions;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ARCANA_API UArcanaActionGroup : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FArcanaAction> Actions;
};
