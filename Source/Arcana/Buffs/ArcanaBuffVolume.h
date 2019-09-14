// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "ArcanaBuffVolume.generated.h"

class UArcanaBuff;
class UArcanaBuffData;

/**
 * 
 */
UCLASS()
class ARCANA_API AArcanaBuffVolume : public ATriggerVolume
{
	GENERATED_BODY()

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UPROPERTY(EditAnywhere, Category = "Buffs")
	TArray<UArcanaBuffData*> BuffsToApply;

private:
	void RemoveAppliedBuffs();

	TArray<UArcanaBuff*> AppliedBuffs;
	
};
