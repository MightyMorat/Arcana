// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ArcanaNeed.generated.h"

UENUM(BlueprintType)
enum class ENeedSatisfaction : uint8
{
	Low,
	Medium,
	High
};

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaNeed
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName NeedId;

	FORCEINLINE bool operator==(const FArcanaNeed& OtherNeed) const	{ return NeedId == OtherNeed.NeedId; }
};

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaNeedDefinition : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UDataAsset* UIData = nullptr;
};

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaNeedState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FArcanaNeed Need;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value = 1.0f;

	UPROPERTY(BlueprintReadOnly)
	float Rate = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	ENeedSatisfaction NeedSatisfaction = ENeedSatisfaction::High;

};