// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaBuffData.h"

#include "Classes/GameplayTagContainer.h"

#if WITH_EDITOR
void UArcanaBuffData::PreSave(const ITargetPlatform* TargetPlatform)
{
	Super::PreSave(TargetPlatform);

	AutogeneratedTags.Reset();

	for (const TPair<FArcanaNeed, float>& NeedRateModifier : NeedRateModifiers)
	{
		const FArcanaNeed& Need = NeedRateModifier.Key;
		const float Value = NeedRateModifier.Value;

		if (Value > 0.0f)
		{
			FString TagNameString = FString::Printf(TEXT("Buffs.%s.Increase"), *Need.NeedId.ToString());
			FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*TagNameString));
			if (Tag.IsValid())
			{
				AutogeneratedTags.AddTag(Tag);
			}
		}
		else if (Value < 0.0f)
		{
			FString TagNameString = FString::Printf(TEXT("Buffs.%s.Decrease"), *Need.NeedId.ToString());
			FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName(*TagNameString));
			if (Tag.IsValid())
			{
				AutogeneratedTags.AddTag(Tag);
			}
		}
	}
}
#endif