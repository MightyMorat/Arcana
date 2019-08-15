// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	AArcanaPlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	int32 GetCurrency() const { return Currency; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	void GetNeedState(const FArcanaNeed& Need, bool& bFound, FArcanaNeedState& NeedState) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	TArray<FArcanaNeed> GetActiveNeeds() const;

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
};
