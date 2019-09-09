// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaPlayerCharacter.h"

#include "Actions/ArcanaAction.h"
#include "Actions/ArcanaActionEvent.h"
#include "AIController.h"
#include "Engine/World.h"
#include "FunctionLibraries/ArcanaFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameMode/ArcanaGameMode.h"
#include "InteractiveObjects/InteractiveObjectComponent.h"

UWorld* UArcanaQueuedAction::GetWorld() const
{
	return TargetInteractiveObjectComponent ? TargetInteractiveObjectComponent->GetWorld() : nullptr;
}

void UArcanaQueuedAction::InitMoveAction(const FVector& InTargetLocation)
{
	Type = EQueuedActionType::MoveTo;
	TargetLocation = InTargetLocation;
}

void UArcanaQueuedAction::InitInteractionAction(const UArcanaActionData* InActionData, UInteractiveObjectComponent* InTargetInteractiveObjectComponent)
{
	Type = EQueuedActionType::ObjectInteraction;
	ActionData = InActionData;
	TargetInteractiveObjectComponent = InTargetInteractiveObjectComponent;

	if (TargetInteractiveObjectComponent)
	{
		TargetLocation = TargetInteractiveObjectComponent->GetComponentLocation();
	}
}

float UArcanaQueuedAction::GetActionProgress() const
{
	if (ActionData && ActionData->bHasMaxDuration)
	{
		return FMath::Clamp(1.0f - ActionRemainingTime / ActionData->MaxDuration, 0.0f, 1.0f);
	}

	return 0.0f;
}

AArcanaPlayerCharacter::AArcanaPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AArcanaPlayerCharacter::CancelQueuedAction(UArcanaQueuedAction* QueuedAction)
{
	if (!QueuedAction)
		return;

	// Clear any movement related to this action
	if (QueuedAction->ActionState == EQueuedActionState::MovingTo)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->ReceiveMoveCompleted.RemoveAll(this);
			AIController->StopMovement();
		}
	}

	// Clear buffs
	AArcanaGameMode* GameMode = Cast<AArcanaGameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode)
	{
		for (UArcanaBuff* Buff : QueuedAction->AppliedActionBuffs)
		{
			GameMode->RemoveBuff(Buff);
		}
	}
	QueuedAction->AppliedActionBuffs.Empty();

	ActionQueue.Remove(QueuedAction);
	QueuedAction->ActionState = EQueuedActionState::Ending;
}

void AArcanaPlayerCharacter::QueueInteractionAction(const UArcanaActionData* ActionData, UInteractiveObjectComponent* TargetInteractiveObjectComponent)
{
	if (ActionQueue.Num() >= MaxQueueSize)
	{
		if (MaxQueueSize > 1)
		{
			// Replace the top of the queue
			CancelQueuedAction(ActionQueue.Last());
		}
		else
		{
			return;
		}
	}

	UArcanaQueuedAction* QueuedAction = NewObject<UArcanaQueuedAction>();
	QueuedAction->InitInteractionAction(ActionData, TargetInteractiveObjectComponent);

	ActionQueue.Add(QueuedAction);
}

void AArcanaPlayerCharacter::QueueMoveAction(const FVector& TargetLocation)
{
	if (ActionQueue.Num() >= MaxQueueSize)
	{
		if (MaxQueueSize > 1)
		{
			// Replace the top of the queue
			CancelQueuedAction(ActionQueue.Last());
		}
		else
		{
			return;
		}
	}

	UArcanaQueuedAction* QueuedAction = NewObject<UArcanaQueuedAction>();
	QueuedAction->InitMoveAction(TargetLocation);

	ActionQueue.Add(QueuedAction);
}

void AArcanaPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UArcanaQueuedAction* CurrentQueuedAction = ActionQueue.Num() > 0 ? ActionQueue[0] : nullptr;
	if (CurrentQueuedAction)
	{
		switch (CurrentQueuedAction->ActionState)
		{
			case EQueuedActionState::NotStarted:
			{
				AAIController* AIController = Cast<AAIController>(GetController());
				if (AIController)
				{
					switch (AIController->MoveToLocation(CurrentQueuedAction->TargetLocation, -1, false))
					{
						case EPathFollowingRequestResult::Type::RequestSuccessful:
						{
							AIController->ReceiveMoveCompleted.AddDynamic(this, &AArcanaPlayerCharacter::OnMoveCompleted);
							CurrentQueuedAction->ActionState = EQueuedActionState::MovingTo;
							break;
						}
						case EPathFollowingRequestResult::Type::AlreadyAtGoal:
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
									BeginInteraction(CurrentQueuedAction);
									break;
								}
							}
							break;
						}
						case EPathFollowingRequestResult::Type::Failed:
						{
							CancelQueuedAction(CurrentQueuedAction);
							break;
						}
					}
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
				const UArcanaActionData* ActionData = CurrentQueuedAction->ActionData;
				if (ActionData)
				{
					if (ActionData->bHasMaxDuration)
					{
						const float DeltaGameHours = UArcanaFunctionLibrary::GetDeltaGameHours(DeltaSeconds);
						CurrentQueuedAction->ActionRemainingTime -= DeltaGameHours;
					}

					// Trigger conditioned events
					for (const UArcanaActionEvent* Event : ActionData->ActionEvents)
					{
						if (Event && Event->GetTriggerType() == EArcanaActionEventTriggerType::OnConditionsMet
							&& !CurrentQueuedAction->TriggeredEvents.Contains(Event) && Event->AreConditionsMet(CurrentQueuedAction))
						{
							CurrentQueuedAction->TriggeredEvents.Add(Event); // Keep track so we don't trigger it more than once
							Event->TriggerEffects(CurrentQueuedAction);
						}
					}

					if (ActionData->bHasMaxDuration && CurrentQueuedAction->ActionRemainingTime <= 0.0f)
					{
						CancelQueuedAction(CurrentQueuedAction);
					}
				}

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

	UArcanaQueuedAction* CurrentQueuedAction = ActionQueue[0];
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

	UArcanaQueuedAction* CurrentQueuedAction = ActionQueue.Num() > 0 ? ActionQueue[0] : nullptr;
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
				BeginInteraction(CurrentQueuedAction);
				break;
			}
		}
	}
}

void AArcanaPlayerCharacter::BeginInteraction(UArcanaQueuedAction* CurrentQueuedAction)
{
	if (!CurrentQueuedAction)
		return;

	if (const UArcanaActionData * ActionData = CurrentQueuedAction->ActionData)
	{
		AArcanaGameMode* GameMode = Cast<AArcanaGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			// Apply action buffs
			for (const UArcanaBuffData* BuffData : ActionData->OngoingBuffs)
			{
				UArcanaBuff* AppliedBuff = GameMode->ApplyBuff(BuffData, CurrentQueuedAction->TargetInteractiveObjectComponent);
				if (AppliedBuff)
				{
					CurrentQueuedAction->AppliedActionBuffs.Add(AppliedBuff);
				}
			}
		}

		// Trigger OnStart events
		for (const UArcanaActionEvent* Event : ActionData->ActionEvents)
		{
			if (Event && Event->GetTriggerType() == EArcanaActionEventTriggerType::OnActionStart && Event->AreConditionsMet(CurrentQueuedAction))
			{
				Event->TriggerEffects(CurrentQueuedAction);
			}
		}

		// Set duration
		if (ActionData->bHasMaxDuration)
		{
			CurrentQueuedAction->ActionRemainingTime = ActionData->MaxDuration;
		}
	}

	CurrentQueuedAction->ActionState = EQueuedActionState::InProgress;
}