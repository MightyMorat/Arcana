// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaPlayerCharacter.h"

#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AArcanaPlayerCharacter::AArcanaPlayerCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AArcanaPlayerCharacter::MoveToLocation(const FVector& TargetLocation)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->MoveToLocation(TargetLocation);
	}
}