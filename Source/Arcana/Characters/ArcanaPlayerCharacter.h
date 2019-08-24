// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArcanaPlayerCharacter.generated.h"

class UArcanaActionData;
class UArcanaBuff;
class UInteractiveObjectComponent;

UCLASS()
class ARCANA_API AArcanaPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AArcanaPlayerCharacter();
	virtual void Tick(float DeltaSeconds);

	void MoveToLocation(const FVector& TargetLocation);

	UFUNCTION(BlueprintCallable)
	void EndCurrentAction();

	UFUNCTION(BlueprintCallable)
	void QueueAction(const UArcanaActionData* ActionData, UInteractiveObjectComponent* TargetInteractiveObjectComponent);

	const UArcanaActionData* GetCurrentActionData() const { return CurrentActionData; }

protected:
	UPROPERTY(BlueprintReadOnly)
	const UArcanaActionData* CurrentActionData;

	float CurrentActionEndTime = 0.0f;

	UPROPERTY()
	TArray<UArcanaBuff*> CurrentActionBuffs;
};
