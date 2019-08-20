// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaTableRowCustomization.h"

class FArcanaSkillCustomization : public FArcanaTableRowCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

protected:
	virtual FString GetPropertyName() const override { return TEXT("SkillId"); }
	virtual UDataTable* GetDataTable() const override;
};
