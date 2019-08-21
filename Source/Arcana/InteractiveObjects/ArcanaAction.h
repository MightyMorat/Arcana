// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArcanaAction.generated.h"

class UArcanaBuffData;
class UArcanaCondition;

UCLASS(BlueprintType)
class ARCANA_API UArcanaActionData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Duration = 10.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<UArcanaBuffData*> OngoingBuffs;
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
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
