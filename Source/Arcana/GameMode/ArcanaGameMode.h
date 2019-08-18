// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Buffs/ArcanaBuffData.h"
#include "GameFramework/GameModeBase.h"
#include "Needs/ArcanaNeed.h"

#include "ArcanaGameMode.generated.h"

class AArcanaPlayerCharacter;

/**
 * 
 */
UCLASS()
class ARCANA_API AArcanaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArcanaGameMode();

	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void Tick(float DeltaSeconds);

	AArcanaPlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	int32 GetCurrency() const { return Currency; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	void GetNeedState(FArcanaNeed Need, bool& bFound, FArcanaNeedState& NeedState) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	TArray<FArcanaNeed> GetActiveNeeds() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	TArray<UArcanaBuffData*> GetActiveBuffs() const { return Buffs; }

protected:
	/** The default pawn class used by players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<class ACharacter> PlayerCharacterClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AArcanaPlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<FArcanaNeedState> NeedStates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	int32 Currency = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<UArcanaBuffData*> Buffs;
};
