// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameInstance.h"

#include "Buffs/ArcanaBuff.h"
#include "Buffs/ArcanaBuffData.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "MoviePlayer/Public/MoviePlayer.h"
#include "Settings/ArcanaSettings.h"

void UArcanaGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UArcanaGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UArcanaGameInstance::EndLoadingScreen);
}

void UArcanaGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
}

void UArcanaGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{

}

void UArcanaGameInstance::LoadScenario(const FArcanaStartingScenario& Scenario)
{
	// Populate need states
	NeedStates.Empty();
	for (const FArcanaNeed& Need : Scenario.StartingNeeds)
	{
		FArcanaNeedState& NeedState = NeedStates.AddDefaulted_GetRef();
		NeedState.Need = Need;
	}

	// Add starting buffs
	for (const UArcanaBuffData* BuffData : Scenario.StartingBuffs)
	{
		UArcanaBuff* Buff = NewObject<UArcanaBuff>();
		Buff->BuffData = BuffData;
		Buff->ContextObject = this;
		Buffs.Add(Buff);
	}

	// Populate skill states
	SkillStates.Empty();
	UDataTable* SkillsDataTable = UArcanaSettings::Get()->SkillsDataTable.LoadSynchronous();
	if (SkillsDataTable)
	{
		const TArray<FName> SkillNames = SkillsDataTable->GetRowNames();
		for (const FName& SkillName : SkillNames)
		{
			FArcanaSkillState& SkillState = SkillStates.AddDefaulted_GetRef();
			SkillState.Skill.SkillId = SkillName;
			
			const int32* SkillStartingValue = Scenario.StartingSkillLevels.Find(SkillState.Skill);
			if (SkillStartingValue)
			{
				SkillState.CurrentLevel = *SkillStartingValue;
			}
		}
	}

	Currency = Scenario.StartingCurrency;

	// Init time
	TotalTimeHours = (float)Scenario.StartingHour;

	// Load level
	UGameplayStatics::OpenLevel(this, Scenario.LevelName);
}