// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SGraphPin.h"
#include "SlateBasics.h"

class SArcanaNeedPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SArcanaNeedPin) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj);
	//this override is used to display slate widget used for customization.
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;

private:
	TSharedRef<SWidget> OnGenerateNeedsComboBox(TSharedPtr<FName> InItem);
	void OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type SelectInfo);
	FText CreateNeedsComboBoxContent() const;

	TSharedPtr<SComboBox<TSharedPtr<FName>>> NeedsComboBox;
	TArray<TSharedPtr<FName>> NeedOptions;
};