// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Navigation/PathFollowingComponent.h"
#include "Containers/Queue.h"
#include "GameFramework/Character.h"
#include "ArcanaPlayerCharacter.generated.h"

class UArcanaActionData;
class UArcanaBuff;
class UInteractiveObjectComponent;

UENUM(BlueprintType)
enum class EQueuedActionType : uint8
{
	MoveTo,
	ObjectInteraction
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
	void GetActionProgress(bool& bHasEndTime, float& ActionProgress) const;

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
	float ActionEndTime = -FLT_MAX;

	UPROPERTY()
	TArray<UArcanaBuff*> AppliedActionBuffs;

	FVector TargetLocation = FVector::ZeroVector;

	void InitMoveAction(const FVector& InTargetLocation);
	void InitInteractionAction(const UArcanaActionData* InActionData, UInteractiveObjectComponent* InTargetInteractiveObjectComponent);
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

	const UArcanaActionData* GetInProgressActionData() const;

protected:
	UFUNCTION()
	void OnMoveCompleted(struct FAIRequestID RequestID, EPathFollowingResult::Type MovementResult);

	void BeginInteraction(UArcanaQueuedAction* CurrentQueuedAction);

	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	int32 MaxQueueSize = 2;

	UPROPERTY(BlueprintReadOnly)
	TArray<UArcanaQueuedAction*> ActionQueue;
};
