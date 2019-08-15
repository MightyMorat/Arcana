// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Needs/ArcanaNeed.h"

#include "ArcanaFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ARCANA_API UArcanaFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	static void GetNeedDefinition(FArcanaNeed Need, bool& bFound, FArcanaNeedDefinition& NeedDefinition);
};
