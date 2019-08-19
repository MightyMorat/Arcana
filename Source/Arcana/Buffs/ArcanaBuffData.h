// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Conditions/ArcanaCondition.h"
#include "Classes/GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Needs/ArcanaNeed.h"

#include "ArcanaBuffData.generated.h"

UENUM()
enum class EBuffUpdateTime : uint8
{
	Default, // Update the buff straight away - tag conditions will be based on the tags at the start of the frame
	LateUpdate // A later update - tag conditions will be based on active buffs with a Default buff update time
};

/**
 * 
 */
UCLASS(BlueprintType, AutoExpandCategories = ("Conditions", "NeedModifiers"))
class ARCANA_API UArcanaBuffData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/** Conditions that must be met or the buff will not be added to the player */
	UPROPERTY(EditAnywhere, Instanced, Category = "Conditions")
	TArray<UArcanaCondition*> ApplicationConditions;

	/** Conditions that must be met for the buff to be considered active */
	UPROPERTY(EditAnywhere, Instanced, Category = "Conditions")
	TArray<UArcanaCondition*> OngoingConditions;

	/** When to update this buff compared to other buffs */
	UPROPERTY(EditAnywhere, Category = "Update")
	EBuffUpdateTime UpdateTime = EBuffUpdateTime::Default;

	UPROPERTY(EditAnywhere, Category = "Tags")
	FGameplayTagContainer BuffTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NeedModifiers")
	TMap<FArcanaNeed, float> NeedRateModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UDataAsset* UIData = nullptr;
};
