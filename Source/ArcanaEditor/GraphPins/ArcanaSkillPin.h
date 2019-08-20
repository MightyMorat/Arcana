// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaTableRowPin.h"

class SArcanaSkillPin : public SArcanaTableRowPin
{
public:
	SLATE_BEGIN_ARGS(SArcanaSkillPin) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);

protected:
	virtual FString GetPropertyName() const override { return TEXT("SkillId"); }
	virtual UDataTable* GetDataTable() const override;
};