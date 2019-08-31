// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameInstance.h"

#include "Engine/Classes/Kismet/GameplayStatics.h"

void UArcanaGameInstance::LoadScenario(const FArcanaStartingScenario& Scenario)
{
	// Populate starting state info

	// Load level
	UGameplayStatics::OpenLevel(this, Scenario.LevelName);
}