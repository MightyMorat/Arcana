// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Conditions/ArcanaCondition.h"
#include "Classes/GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Needs/ArcanaNeed.h"
#include "Skills/ArcanaSkill.h"

#include "ArcanaBuffData.generated.h"

class UTexture2D;

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
#if WITH_EDITOR
	virtual void PreSave(const class ITargetPlatform* TargetPlatform);
#endif

	/** Conditions that must be met or the buff will not be added to the player */
	UPROPERTY(EditAnywhere, Instanced, Category = "Conditions")
	TArray<UArcanaCondition*> ApplicationConditions;

	/** Conditions that must be met for the buff to be considered active */
	UPROPERTY(EditAnywhere, Instanced, Category = "Conditions")
	TArray<UArcanaCondition*> OngoingConditions;

	/** When to update this buff compared to other buffs */
	UPROPERTY(EditAnywhere, Category = "Update")
	EBuffUpdateTime UpdateTime = EBuffUpdateTime::Default;

	UPROPERTY(VisibleAnywhere, Category = "Buff Tags")
	FGameplayTagContainer AutogeneratedTags;

	UPROPERTY(EditAnywhere, Category = "Buff Tags")
	FGameplayTagContainer AdditionalTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NeedModifiers")
	TMap<FArcanaNeed, float> NeedRateModifiers;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillModifiers")
	TMap<FArcanaSkill, float> SkillRateModifiers;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	FText Name;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	FText Description;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "UI")
	UTexture2D* IconTexture;
};
