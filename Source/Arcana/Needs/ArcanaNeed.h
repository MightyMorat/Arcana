// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanaNeed.generated.h"

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaNeed
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NeedId;
};

USTRUCT(BlueprintType)
struct ARCANA_API FArcanaNeedDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName Name = NAME_None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FName Description = NAME_None;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* IconTexture = nullptr;
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

};