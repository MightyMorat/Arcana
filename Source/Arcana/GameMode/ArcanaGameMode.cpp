// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameMode.h"

#include "Characters/ArcanaPlayerCharacter.h"
#include "Engine/World.h"
#include "Settings/ArcanaSettings.h"

AArcanaGameMode::AArcanaGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

APawn* AArcanaGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	// Also spawn the player character at the spawn transform
	if (PlayerCharacterClass)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Instigator = Instigator;
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save the player character into a map
		PlayerCharacter = GetWorld()->SpawnActor<AArcanaPlayerCharacter>(PlayerCharacterClass, SpawnTransform, SpawnInfo);
	}

	return Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
}

void AArcanaGameMode::GetNeedState(FArcanaNeed Need, bool& bFound, FArcanaNeedState& OutNeedState) const
{
	bFound = false;
	OutNeedState = FArcanaNeedState();

	for (const FArcanaNeedState& NeedState : NeedStates)
	{
		if (NeedState.Need == Need)
		{
			bFound = true;
			OutNeedState = NeedState;
			return;
		}
	}
}

TArray<FArcanaNeed> AArcanaGameMode::GetActiveNeeds() const
{
	TArray<FArcanaNeed> ActiveNeeds;
	for (const FArcanaNeedState& NeedState : NeedStates)
	{
		ActiveNeeds.Add(NeedState.Need);
	}

	return ActiveNeeds;
}

void AArcanaGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const UArcanaSettings* ArcanaSettings = UArcanaSettings::Get();

	for (FArcanaNeedState& NeedState : NeedStates)
	{
		// todo[hale] - calculate rate from buffs

		// Update value based on current rate
		NeedState.Value += DeltaSeconds*NeedState.Rate;
		NeedState.Value = FMath::Clamp(NeedState.Value, 0.0f, 1.0f);

		// Calculate satisfaction from value thresholds
		if (NeedState.Value >= ArcanaSettings->HighNeedThreshold)
		{
			NeedState.NeedSatisfaction = ENeedSatisfaction::High;
		}
		else if (NeedState.Value >= ArcanaSettings->MediumNeedThreshold)
		{
			NeedState.NeedSatisfaction = ENeedSatisfaction::Medium;
		}
		else
		{
			NeedState.NeedSatisfaction = ENeedSatisfaction::Low;
		}
	}
}