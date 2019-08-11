// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

	/** The default pawn class used by players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<class ACharacter> PlayerCharacterClass;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AArcanaPlayerCharacter* PlayerCharacter = nullptr;
	
};
