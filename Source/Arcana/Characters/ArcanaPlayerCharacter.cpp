// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaPlayerCharacter.h"

#include "AIController.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/ArcanaGameMode.h"
#include "InteractiveObjects/ArcanaAction.h"
#include "InteractiveObjects/InteractiveObjectComponent.h"

AArcanaPlayerCharacter::AArcanaPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
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

void AArcanaPlayerCharacter::EndCurrentAction()
{
	CurrentActionData = nullptr;

	AArcanaGameMode* GameMode = Cast<AArcanaGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		for (UArcanaBuff* Buff : CurrentActionBuffs)
		{
			GameMode->RemoveBuff(Buff);
		}
	}

	CurrentActionBuffs.Empty();
}

void AArcanaPlayerCharacter::QueueAction(const UArcanaActionData* ActionData, UInteractiveObjectComponent* TargetInteractiveObjectComponent)
{
	if (!TargetInteractiveObjectComponent || !ActionData)
		return;

	AArcanaGameMode* GameMode = Cast<AArcanaGameMode>(GetWorld()->GetAuthGameMode());
	if (!GameMode)
		return;

	EndCurrentAction();

	CurrentActionData = ActionData;
	CurrentActionEndTime = GetWorld()->GetTimeSeconds() + ActionData->Duration;

	for (const UArcanaBuffData* BuffData : ActionData->OngoingBuffs)
	{
		UArcanaBuff* AppliedBuff = GameMode->ApplyBuff(BuffData, TargetInteractiveObjectComponent);
		if (AppliedBuff)
		{
			CurrentActionBuffs.Add(AppliedBuff);
		}
	}
}

void AArcanaPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentActionData)
	{
		if (CurrentActionEndTime <= GetWorld()->GetTimeSeconds())
		{
			EndCurrentAction();
		}
	}
}