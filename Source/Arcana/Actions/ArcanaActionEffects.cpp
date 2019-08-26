// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaActionEffects.h"

#include "Characters/ArcanaPlayerCharacter.h"
#include "Engine/World.h"
#include "GameMode/ArcanaGameMode.h"

void UArcanaEndActionEffect::TriggerEffect(UArcanaQueuedAction* QueuedAction) const
{
	UWorld* World = QueuedAction ? QueuedAction->GetWorld() : nullptr;
	AArcanaGameMode* GameMode = World ? Cast<AArcanaGameMode>(World->GetAuthGameMode()) : nullptr;
	AArcanaPlayerCharacter* PlayerCharacter = GameMode ? GameMode->GetPlayerCharacter() : nullptr;
	if (PlayerCharacter)
	{
		PlayerCharacter->CancelQueuedAction(QueuedAction);
	}
}