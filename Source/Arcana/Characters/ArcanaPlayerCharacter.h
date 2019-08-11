// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArcanaPlayerCharacter.generated.h"

UCLASS()
class ARCANA_API AArcanaPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AArcanaPlayerCharacter();

	void MoveToLocation(const FVector& TargetLocation);
};
