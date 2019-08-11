// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameMode.h"

#include "Characters/ArcanaPlayerCharacter.h"
#include "Engine/World.h"

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