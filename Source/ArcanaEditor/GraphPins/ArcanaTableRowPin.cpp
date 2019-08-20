// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaTableRowPin.h"

#include "DetailLayoutBuilder.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "KismetEditorUtilities.h"
#include "Settings/ArcanaSettings.h"
#include "STextComboBox.h"

void SArcanaTableRowPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	Options.Empty();
	Options.Add(MakeShareable(new FName(TEXT("None"))));

	UDataTable* DataTable = GetDataTable();
	if (DataTable)
	{
		TArray<FName> RowNames = DataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			Options.Add(MakeShareable(new FName(RowName)));
		}
	}

	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}

TSharedRef<SWidget>	SArcanaTableRowPin::GetDefaultValueWidget()
{
	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();

	FName CurrentlySelectedOption;
	int32 CurrentlySelectedIndex = 0;
	for (int32 i = 0; i < Options.Num(); ++i)
	{
		if (CurrentDefaultValue.Contains((*Options[i].Get()).ToString()))
		{
			CurrentlySelectedIndex = i;
			break;
		}
	}

	return SAssignNew(ComboBox, SComboBox<TSharedPtr<FName>>)
		.OptionsSource(&Options)
		.InitiallySelectedItem(Options[CurrentlySelectedIndex])
		.OnGenerateWidget(this, &SArcanaTableRowPin::OnGenerateComboBox)
		.OnSelectionChanged(this, &SArcanaTableRowPin::OnSelectionChanged)
		.Visibility(this, &SGraphPin::GetDefaultValueVisibility)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &SArcanaTableRowPin::CreateComboBoxContent)
			.Font(IDetailLayoutBuilder::GetDetailFont())
		];
}

TSharedRef<SWidget> SArcanaTableRowPin::OnGenerateComboBox(TSharedPtr<FName> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromName(*InItem.Get()));
}

void SArcanaTableRowPin::OnSelectionChanged(TSharedPtr<FName> ItemSelected, ESelectInfo::Type SelectInfo)
{
	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();
	FString attribute = (*ItemSelected).ToString();

	FString AttributeString = FString::Printf(TEXT("(%s=\"%s\")"), *GetPropertyName(), *attribute);	
	if (!CurrentDefaultValue.Equals(AttributeString))
	{
		GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, AttributeString);
	}
}

FText SArcanaTableRowPin::CreateComboBoxContent() const
{
	const bool bHasSelectedItem = ComboBox.IsValid() && ComboBox->GetSelectedItem().IsValid();

	return bHasSelectedItem ? FText::FromName(*ComboBox->GetSelectedItem().Get()) : FText();
}