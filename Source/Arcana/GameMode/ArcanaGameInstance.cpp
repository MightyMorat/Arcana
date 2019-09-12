// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaGameInstance.h"

#include "Buffs/ArcanaBuff.h"
#include "Buffs/ArcanaBuffData.h"
#include "Engine/Classes/Engine/Texture2D.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "MoviePlayer/Public/MoviePlayer.h"
#include "Settings/ArcanaSettings.h"
#include "UObject/ConstructorHelpers.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SOverlay.h"

/** SArcanaLoadingScreenWidget */
class SArcanaLoadingScreenWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SArcanaLoadingScreenWidget)
		: _LoadingScreenImage( nullptr )
	{}
		SLATE_ARGUMENT(UTexture2D*, LoadingScreenImage)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		if (InArgs._LoadingScreenImage)
		{
			ImageBrush.SetResourceObject(InArgs._LoadingScreenImage);
			ImageBrush.ImageSize.X = InArgs._LoadingScreenImage->GetSurfaceWidth();
			ImageBrush.ImageSize.Y = InArgs._LoadingScreenImage->GetSurfaceHeight();
			ImageBrush.DrawAs = ESlateBrushDrawType::Image;
		}

		ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SImage)
				.Image(&ImageBrush)
			]
			+SOverlay::Slot()
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			[
				SNew(SThrobber)
			]
		];
	}

private:
	FSlateBrush ImageBrush;
};

/** UArcanaGameInstance */
UArcanaGameInstance::UArcanaGameInstance()
{
	LoadingScreenTextures.Empty();

	UArcanaSettings* Settings = UArcanaSettings::Get();
	for (const TSoftObjectPtr<UTexture2D>& SoftObjectPtr : Settings->LoadingScreenTextures)
	{
		LoadingScreenTextures.Add(SoftObjectPtr.LoadSynchronous());
	}
}

void UArcanaGameInstance::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UArcanaGameInstance::BeginLoadingScreen);
	//FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UArcanaGameInstance::EndLoadingScreen);
}

void UArcanaGameInstance::BeginLoadingScreen(const FString& InMapName)
{
	UTexture2D* LoadingScreenImage = nullptr;
	if (LoadingScreenTextures.Num() > 0)
	{
		LoadingScreenImage = LoadingScreenTextures[FMath::RandRange(0, LoadingScreenTextures.Num() - 1)];
	}

	FLoadingScreenAttributes LoadingScreen;
	LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
	LoadingScreen.MinimumLoadingScreenDisplayTime = UArcanaSettings::Get()->MinimumLoadingScreenTime;
	LoadingScreen.WidgetLoadingScreen = SNew(SArcanaLoadingScreenWidget).LoadingScreenImage(LoadingScreenImage);

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
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

	bIsScenarioLoaded = true;

	// Load level
	UGameplayStatics::OpenLevel(this, Scenario.LevelName);
}