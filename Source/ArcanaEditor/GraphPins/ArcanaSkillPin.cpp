// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaSkillPin.h"

#include "Settings/ArcanaSettings.h"

void SArcanaSkillPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SArcanaTableRowPin::Construct(SArcanaTableRowPin::FArguments(), InGraphPinObj);
}

UDataTable* SArcanaSkillPin::GetDataTable() const
{
	return UArcanaSettings::Get()->SkillsDataTable.LoadSynchronous();
}