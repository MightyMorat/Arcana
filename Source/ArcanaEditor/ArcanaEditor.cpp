// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaEditor.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_GAME_MODULE(FArcanaEditorModule, ArcanaEditor);

DEFINE_LOG_CATEGORY(ArcanaEditor)

#define LOCTEXT_NAMESPACE "ArcanaEditor"

void FArcanaEditorModule::StartupModule()
{
	UE_LOG(ArcanaEditor, Display, TEXT("Module Started"));
}

void FArcanaEditorModule::ShutdownModule()
{
	UE_LOG(ArcanaEditor, Display, TEXT("Module Shutdown"));
}

#undef LOCTEXT_NAMESPACE