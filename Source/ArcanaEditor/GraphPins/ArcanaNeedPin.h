// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaTableRowPin.h"

class SArcanaNeedPin : public SArcanaTableRowPin
{
public:
	SLATE_BEGIN_ARGS(SArcanaNeedPin) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:
	virtual FString GetPropertyName() const override { return TEXT("NeedId"); }
	virtual UDataTable* GetDataTable() const override;
};