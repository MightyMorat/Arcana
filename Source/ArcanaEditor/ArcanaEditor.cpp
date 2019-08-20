// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaEditor.h"
#include "EdGraphUtilities.h"
#include "GraphPins/ArcanaGraphPinFactory.h"
#include "Modules/ModuleManager.h"
#include "DetailCustomization/ArcanaNeedCustomization.h"
#include "DetailCustomization/ArcanaSkillCustomization.h"
#include "PropertyEditorModule.h"

IMPLEMENT_GAME_MODULE(FArcanaEditorModule, ArcanaEditor);

DEFINE_LOG_CATEGORY(ArcanaEditor)

#define LOCTEXT_NAMESPACE "ArcanaEditor"

void FArcanaEditorModule::StartupModule()
{
	UE_LOG(ArcanaEditor, Display, TEXT("Module Started"));

	// Custom properties
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomPropertyTypeLayout("ArcanaNeed", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FArcanaNeedCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout("ArcanaSkill", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FArcanaSkillCustomization::MakeInstance));

	// Custom pins
	TSharedPtr<FArcanaGraphPinFactory> ArcanaGraphPinFactory = MakeShareable(new FArcanaGraphPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(ArcanaGraphPinFactory);
}

void FArcanaEditorModule::ShutdownModule()
{
	UE_LOG(ArcanaEditor, Display, TEXT("Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE