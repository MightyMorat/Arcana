// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaPlayerCharacter.h"

#include "AIController.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/ArcanaGameMode.h"
#include "InteractiveObjects/ArcanaAction.h"
#include "InteractiveObjects/InteractiveObjectComponent.h"

void UQueuedAction::InitMoveAction(const FVector& InTargetLocation)
{
	Type = EQueuedActionType::MoveTo;
	TargetLocation = InTargetLocation;
}

void UQueuedAction::InitInteractionAction(const UArcanaActionData* InActionData, UInteractiveObjectComponent* InTargetInteractiveObjectComponent)
{
	Type = EQueuedActionType::ObjectInteraction;
	ActionData = InActionData;
	TargetInteractiveObjectComponent = InTargetInteractiveObjectComponent;

	// todo[hale] - calculate target location
}

AArcanaPlayerCharacter::AArcanaPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AArcanaPlayerCharacter::CancelQueuedAction(UQueuedAction* QueuedAction)
{
	if (!QueuedAction)
		return;

	if (ActionQueue.Num() > 0 && ActionQueue[0] == QueuedAction)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->ReceiveMoveCompleted.RemoveAll(this);
			AIController->StopMovement();
		}
	}

	ActionQueue.Remove(QueuedAction);
	QueuedAction->ActionState = EQueuedActionState::Ending;
}

//void AArcanaPlayerCharacter::EndCurrentAction()
//{
//	CurrentActionData = nullptr;
//
//	AArcanaGameMode* GameMode = Cast<AArcanaGameMode>(GetWorld()->GetAuthGameMode());
//	if (GameMode)
//	{
//		for (UArcanaBuff* Buff : CurrentActionBuffs)
//		{
//			GameMode->RemoveBuff(Buff);
//		}
//	}
//
//	CurrentActionBuffs.Empty();
//}
//
//void AArcanaPlayerCharacter::QueueAction(const UArcanaActionData* ActionData, UInteractiveObjectComponent* TargetInteractiveObjectComponent)
//{
//	if (!TargetInteractiveObjectComponent || !ActionData)
//		return;
//
//	AArcanaGameMode* GameMode = Cast<AArcanaGameMode>(GetWorld()->GetAuthGameMode());
//	if (!GameMode)
//		return;
//
//	EndCurrentAction();
//
//	CurrentActionData = ActionData;
//	CurrentActionEndTime = GetWorld()->GetTimeSeconds() + ActionData->Duration;
//
//	for (const UArcanaBuffData* BuffData : ActionData->OngoingBuffs)
//	{
//		UArcanaBuff* AppliedBuff = GameMode->ApplyBuff(BuffData, TargetInteractiveObjectComponent);
//		if (AppliedBuff)
//		{
//			CurrentActionBuffs.Add(AppliedBuff);
//		}
//	}
//}

void AArcanaPlayerCharacter::QueueInteractionAction(const UArcanaActionData* ActionData, UInteractiveObjectComponent* TargetInteractiveObjectComponent)
{
	if (ActionQueue.Num() >= MaxQueueSize)
		return; // todo[hale] replace top of queue

	UQueuedAction* QueuedAction = NewObject<UQueuedAction>();
	QueuedAction->InitInteractionAction(ActionData, TargetInteractiveObjectComponent);

	ActionQueue.Add(QueuedAction);
}

void AArcanaPlayerCharacter::QueueMoveAction(const FVector& TargetLocation)
{
	if (ActionQueue.Num() >= MaxQueueSize)
		return; // todo[hale] replace top of queue

	UQueuedAction* QueuedAction = NewObject<UQueuedAction>();
	QueuedAction->InitMoveAction(TargetLocation);

	ActionQueue.Add(QueuedAction);
}

void AArcanaPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	/*if (CurrentActionData)
	{
		if (CurrentActionEndTime <= GetWorld()->GetTimeSeconds())
		{
			EndCurrentAction();
		}
	}*/

	UQueuedAction* CurrentQueuedAction = ActionQueue.Num() > 0 ? ActionQueue[0] : nullptr;
	if (CurrentQueuedAction)
	{
		switch (CurrentQueuedAction->ActionState)
		{
			case EQueuedActionState::NotStarted:
			{
				// todo[hale] - move for interactions too
				if (CurrentQueuedAction->Type == EQueuedActionType::MoveTo)
				{
					AAIController* AIController = Cast<AAIController>(GetController());
					if (AIController)
					{
						switch (AIController->MoveToLocation(CurrentQueuedAction->TargetLocation))
						{
							case EPathFollowingRequestResult::Type::RequestSuccessful:
							{
								AIController->ReceiveMoveCompleted.AddDynamic(this, &AArcanaPlayerCharacter::OnMoveCompleted);
								CurrentQueuedAction->ActionState = EQueuedActionState::MovingTo;
								break;
							}
							case EPathFollowingRequestResult::Type::AlreadyAtGoal:
							{
								CurrentQueuedAction->ActionState = EQueuedActionState::MovingTo;
								break;
							}
							case EPathFollowingRequestResult::Type::Failed:
							{
								CancelQueuedAction(CurrentQueuedAction);
								break;
							}
						}
					}
				}
				else
				{
					CurrentQueuedAction->ActionState = EQueuedActionState::InProgress;
				}

				break;
			}
			case EQueuedActionState::MovingTo:
			{
				// Wait for the callback to say movement is done
				break;
			}
			case EQueuedActionState::InProgress:
			{
				// todo[hale] - stop if duration reached
				break;
			}
			case EQueuedActionState::Ending:
			{
				CancelQueuedAction(CurrentQueuedAction);
				break;
			}
		}
	}
}

const UArcanaActionData* AArcanaPlayerCharacter::GetInProgressActionData() const
{
	if (ActionQueue.Num() == 0)
		return nullptr;

	UQueuedAction* CurrentQueuedAction = ActionQueue[0];
	if (CurrentQueuedAction->ActionState == EQueuedActionState::InProgress)
	{
		return CurrentQueuedAction->ActionData;
	}

	return nullptr;
}

void AArcanaPlayerCharacter::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type MovementResult)
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->ReceiveMoveCompleted.RemoveAll(this);
	}

	UQueuedAction* CurrentQueuedAction = ActionQueue.Num() > 0 ? ActionQueue[0] : nullptr;
	if (CurrentQueuedAction && CurrentQueuedAction->ActionState == EQueuedActionState::MovingTo)
	{
		switch (CurrentQueuedAction->Type)
		{
			case EQueuedActionType::MoveTo:
			{
				CancelQueuedAction(CurrentQueuedAction);
				break;
			}
			case EQueuedActionType::ObjectInteraction:
			{
				CurrentQueuedAction->ActionState = EQueuedActionState::InProgress;
				break;
			}
		}
	}
}