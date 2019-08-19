// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ArcanaCondition.generated.h"

/**
 * 
 */
UCLASS(DefaultToInstanced, EditInlineNew)
class ARCANA_API UArcanaCondition : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool bNegateCondition = false;

	bool IsConditionMet() const { return IsConditionMet_Internal() != bNegateCondition; }

protected:
	virtual bool IsConditionMet_Internal() const { return true; }

};
