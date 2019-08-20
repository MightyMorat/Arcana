// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArcanaActionGroup.generated.h"

class UArcanaCondition;

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
