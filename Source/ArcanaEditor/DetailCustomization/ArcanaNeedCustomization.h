// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArcanaTableRowCustomization.h"

class FArcanaNeedCustomization : public FArcanaTableRowCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

protected:
	virtual FString GetPropertyName() const override { return TEXT("NeedId"); }
	virtual UDataTable* GetDataTable() const override;
};
