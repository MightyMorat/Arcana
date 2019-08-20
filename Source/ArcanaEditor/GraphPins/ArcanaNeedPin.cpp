// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaNeedPin.h"

#include "Settings/ArcanaSettings.h"

void SArcanaNeedPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	SArcanaTableRowPin::Construct(SArcanaTableRowPin::FArguments(), InGraphPinObj);
}

UDataTable* SArcanaNeedPin::GetDataTable() const
{
	return UArcanaSettings::Get()->NeedsDataTable.LoadSynchronous();
}