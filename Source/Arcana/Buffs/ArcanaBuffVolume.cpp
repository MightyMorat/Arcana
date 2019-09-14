// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaBuffVolume.h"

#include "Engine/World.h"
#include "GameMode/ArcanaGameMode.h"

void AArcanaBuffVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	RemoveAppliedBuffs();

	UWorld* World = GetWorld();
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;
	if (GameMode)
	{
		for (const UArcanaBuffData* BuffData : BuffsToApply)
		{
			UArcanaBuff* AppliedBuff = GameMode->ApplyBuff(BuffData, this, true);
			if (AppliedBuff)
			{
				AppliedBuffs.Add(AppliedBuff);
			}
		}
	}
}

void AArcanaBuffVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	RemoveAppliedBuffs();
}

void AArcanaBuffVolume::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	RemoveAppliedBuffs();
}

void AArcanaBuffVolume::RemoveAppliedBuffs()
{
	if (AppliedBuffs.Num() == 0)
		return;

	UWorld* World = GetWorld();
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;
	if (GameMode)
	{
		for (UArcanaBuff* Buff : AppliedBuffs)
		{
			GameMode->RemoveBuff(Buff);
		}
	}

	AppliedBuffs.Empty();
}