// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SGraphPin.h"
#include "SlateBasics.h"

class UDataTable;

class SArcanaTableRowPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SArcanaTableRowPin) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);
	//this override is used to display slate widget used for customization.
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;

protected:
	virtual FString GetPropertyName() const = 0;
	virtual UDataTable* GetDataTable() const = 0;

private:
	TSharedRef<SWidget> OnGenerateComboBox(TSharedPtr<FName> InItem);
	void OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type SelectInfo);
	FText CreateComboBoxContent() const;

	TSharedPtr<SComboBox<TSharedPtr<FName>>> ComboBox;
	TArray<TSharedPtr<FName>> Options;
};