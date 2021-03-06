// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Navigation/PathFollowingComponent.h"
#include "Containers/Queue.h"
#include "GameFramework/Character.h"
#include "ArcanaPlayerCharacter.generated.h"

class UArcanaActionData;
class UArcanaActionEvent;
class UArcanaBuff;
class UInteractiveObjectComponent;

UENUM(BlueprintType)
enum class EQueuedActionType : uint8
{
	MoveTo,
	ObjectInteraction,
	Inspect
};

UENUM(BlueprintType)
enum class EQueuedActionState : uint8
{
	NotStarted,
	MovingTo,
	InProgress,
	Ending
};

UCLASS(BlueprintType)
class UArcanaQueuedAction : public UObject
{
	GENERATED_BODY()

	friend class AArcanaPlayerCharacter;

public:
	virtual UWorld* GetWorld() const override;

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	float GetActionProgress() const;

	UPROPERTY(BlueprintReadOnly)
	EQueuedActionType Type = EQueuedActionType::MoveTo;

	UPROPERTY(BlueprintReadOnly)
	const UArcanaActionData* ActionData = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UInteractiveObjectComponent* TargetInteractiveObjectComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	EQueuedActionState ActionState = EQueuedActionState::NotStarted;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInterruptible = true;

	UPROPERTY(BlueprintReadOnly)
	float ActionRemainingTime = -FLT_MAX;

	UPROPERTY()
	TArray<UArcanaBuff*> AppliedActionBuffs;

	TArray<const UArcanaActionEvent*> TriggeredEvents;

	FVector TargetLocation = FVector::ZeroVector;

	void InitMoveAction(const FVector& InTargetLocation);
	void InitInteractionAction(const UArcanaActionData* InActionData, UInteractiveObjectComponent* InTargetInteractiveObjectComponent);
	void InitInspectAction(UInteractiveObjectComponent* InTargetInteractiveObjectComponent);
};

UCLASS()
class ARCANA_API AArcanaPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AArcanaPlayerCharacter();
	virtual void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void CancelQueuedAction(UArcanaQueuedAction* QueuedAction);

	UFUNCTION(BlueprintCallable)
	void QueueInteractionAction(const UArcanaActionData* ActionData, UInteractiveObjectComponent* TargetInteractiveObjectComponent);

	UFUNCTION(BlueprintCallable)
	void QueueMoveAction(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable)
	void QueueInspectAction(UInteractiveObjectComponent* TargetInteractiveObjectComponent);

	const UArcanaActionData* GetInProgressActionData() const;

protected:
	UFUNCTION()
	void OnMoveCompleted(struct FAIRequestID RequestID, EPathFollowingResult::Type MovementResult);

	void BeginInteraction(UArcanaQueuedAction* CurrentQueuedAction);
	void InspectObject(const UInteractiveObjectComponent* TargetInteractiveObjectComponent);

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	int32 MaxQueueSize = 2;

	UPROPERTY(BlueprintReadOnly)
	TArray<UArcanaQueuedAction*> ActionQueue;
};
