// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaSkillCustomization.h"

#include "Settings/ArcanaSettings.h"

TSharedRef<IPropertyTypeCustomization> FArcanaSkillCustomization::MakeInstance()
{
	return MakeShareable(new FArcanaSkillCustomization());
}

UDataTable* FArcanaSkillCustomization::GetDataTable() const
{
	return UArcanaSettings::Get()->SkillsDataTable.LoadSynchronous();
}
	