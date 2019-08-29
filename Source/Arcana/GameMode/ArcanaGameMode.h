// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "Needs/ArcanaNeed.h"
#include "Skills/ArcanaSkill.h"

#include "ArcanaGameMode.generated.h"

class AArcanaPlayerCharacter;
enum class EBuffUpdateTime : uint8;
struct FGameplayTagContainer;
class UArcanaBuff;
class UArcanaBuffData;

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
	virtual void BeginPlay();

	UFUNCTION(BlueprintCallable)
	AArcanaPlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	int32 GetCurrency() const { return Currency; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	void GetNeedState(FArcanaNeed Need, bool& bFound, FArcanaNeedState& NeedState) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	TArray<FArcanaNeed> GetActiveNeeds() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	void GetSkillState(FArcanaSkill Skill, bool& bFound, FArcanaSkillState& SkillState) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	TArray<FArcanaSkill> GetActiveSkills() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Gameplay")
	TArray<UArcanaBuff*> GetActiveBuffs(FArcanaNeed AffectedNeed) const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	UArcanaBuff* ApplyBuff(const UArcanaBuffData* BuffData, UObject* ContextObject);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void RemoveBuff(UArcanaBuff* Buff) { Buffs.Remove(Buff); }

	const FGameplayTagContainer& GetActiveBuffTags() const { return ActiveBuffTags; }

protected:
	FGameplayTagContainer UpdateBuffs(EBuffUpdateTime UpdateTime);

	/** The default pawn class used by players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<class ACharacter> PlayerCharacterClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AArcanaPlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<FArcanaNeedState> NeedStates;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	TArray<FArcanaSkillState> SkillStates;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay", meta = (AllowPrivateAccess = "true"))
	int32 Currency = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	TArray<const UArcanaBuffData*> StartingBuffs;

	UPROPERTY()
	TArray<UArcanaBuff*> Buffs;

	UPROPERTY(VisibleInstanceOnly, Category = "Debug Display")
	FGameplayTagContainer ActiveBuffTags;

private:
	/** Console commands */
	UFUNCTION(Exec)
	void SetNeedValue(FName Need, float Value);

	UFUNCTION(Exec)
	void SetSkillLevel(FName Skill, int32 Level);
};
