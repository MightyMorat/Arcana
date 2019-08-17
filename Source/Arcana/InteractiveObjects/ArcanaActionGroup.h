// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArcanaActionGroup.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ARCANA_API UArcanaActionGroup : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<FName> Actions;
};
