// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/GameplayTagContainer.h"
#include "GameFramework/GameModeBase.h"
#include "Needs/ArcanaNeed.h"
#include "Skills/ArcanaSkill.h"
#include "Time/ArcanaTime.h"

#include "ArcanaGameMode.generated.h"

class AArcanaPlayerCharacter;
enum class EBuffUpdateTime : uint8;
struct FGameplayTagContainer;
class UArcanaBuff;
class UArcanaBuffData;
class UArcanaGameInstance;

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

	UFUNCTION(BlueprintCallable, Category = "Currency")
	int32 GetCurrency() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Needs")
	void GetNeedState(FArcanaNeed Need, bool& bFound, FArcanaNeedState& NeedState) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Needs")
	TArray<FArcanaNeed> GetActiveNeeds() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Skills")
	void GetSkillState(FArcanaSkill Skill, bool& bFound, FArcanaSkillState& SkillState) const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Skills")
	TArray<FArcanaSkill> GetActiveSkills() const;

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Buffs")
	TArray<UArcanaBuff*> GetActiveBuffs(FArcanaNeed AffectedNeed) const;

	UFUNCTION(BlueprintCallable, Category = "Buffs")
	UArcanaBuff* ApplyBuff(const UArcanaBuffData* BuffData, UObject* ContextObject, bool bIsTransient = false);

	UFUNCTION(BlueprintCallable, Category = "Buffs")
	void RemoveBuff(UArcanaBuff* Buff);

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Time")
	FArcanaTimeInfo GetTimeInfo() const;

	const FGameplayTagContainer& GetActiveBuffTags() const { return ActiveBuffTags; }

protected:
	FGameplayTagContainer UpdateBuffs(EBuffUpdateTime UpdateTime);

	/** The default pawn class used by players. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<class ACharacter> PlayerCharacterClass;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	AArcanaPlayerCharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	UArcanaGameInstance* GameInstance = nullptr;

	UPROPERTY(VisibleInstanceOnly, Category = "Debug Display")
	FGameplayTagContainer ActiveBuffTags;

private:
	/** Console commands */
	UFUNCTION(Exec)
	void SetNeedValue(FName Need, float Value);

	UFUNCTION(Exec)
	void SetSkillLevel(FName Skill, int32 Level);
};
