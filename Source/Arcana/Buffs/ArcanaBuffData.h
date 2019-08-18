// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Needs/ArcanaNeed.h"

#include "ArcanaBuffData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ARCANA_API UArcanaBuffData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer BuffTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NeedModifiers")
	TMap<FArcanaNeed, float> NeedRateModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UDataAsset* UIData = nullptr;
};
