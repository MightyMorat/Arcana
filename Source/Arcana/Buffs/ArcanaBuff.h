// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArcanaBuff.generated.h"

class UArcanaBuffData;

/**
 * 
 */
UCLASS(BlueprintType)
class ARCANA_API UArcanaBuff : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	bool bIsActive = false;

	UPROPERTY(BlueprintReadOnly)
	UArcanaBuffData* BuffData = nullptr;
};
