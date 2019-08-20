// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaNeedCustomization.h"

#include "Settings/ArcanaSettings.h"

TSharedRef<IPropertyTypeCustomization> FArcanaNeedCustomization::MakeInstance()
{
	return MakeShareable(new FArcanaNeedCustomization());
}

UDataTable* FArcanaNeedCustomization::GetDataTable() const
{
	return UArcanaSettings::Get()->NeedsDataTable.LoadSynchronous();
}
	