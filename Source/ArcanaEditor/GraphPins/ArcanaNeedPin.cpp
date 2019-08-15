// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaNeedPin.h"

#include "DetailLayoutBuilder.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "KismetEditorUtilities.h"
#include "Settings/ArcanaSettings.h"
#include "STextComboBox.h"

void SArcanaNeedPin::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj)
{
	NeedOptions.Empty();
	NeedOptions.Add(MakeShareable(new FName(TEXT("None"))));

	UDataTable* NeedsDataTable = UArcanaSettings::Get()->NeedsDataTable.LoadSynchronous();
	if (NeedsDataTable)
	{
		TArray<FName> NeedIds = NeedsDataTable->GetRowNames();
		for (const FName& NeedId : NeedIds)
		{
			NeedOptions.Add(MakeShareable(new FName(NeedId)));
		}
	}

	SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObj);
}
TSharedRef<SWidget>	SArcanaNeedPin::GetDefaultValueWidget()
{
	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();

	FName CurrentlySelectedNeed;
	int32 CurrentlySelectedIndex = 0;
	for (int32 i = 0; i < NeedOptions.Num(); ++i)
	{
		if (CurrentDefaultValue.Contains((*NeedOptions[i].Get()).ToString()))
		{
			CurrentlySelectedIndex = i;
			break;
		}
	}

	return SAssignNew(NeedsComboBox, SComboBox<TSharedPtr<FName>>)
		.OptionsSource(&NeedOptions)
		.InitiallySelectedItem(NeedOptions[CurrentlySelectedIndex])
		.OnGenerateWidget(this, &SArcanaNeedPin::OnGenerateNeedsComboBox)
		.OnSelectionChanged(this, &SArcanaNeedPin::OnSelectionChanged)
		.Visibility(this, &SGraphPin::GetDefaultValueVisibility)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &SArcanaNeedPin::CreateNeedsComboBoxContent)
			.Font(IDetailLayoutBuilder::GetDetailFont())
		];
}

TSharedRef<SWidget> SArcanaNeedPin::OnGenerateNeedsComboBox(TSharedPtr<FName> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromName(*InItem.Get()));
}

void SArcanaNeedPin::OnSelectionChanged(TSharedPtr<FName> ItemSelected, ESelectInfo::Type SelectInfo)
{
	FString CurrentDefaultValue = GraphPinObj->GetDefaultAsString();
	FString attribute = (*ItemSelected).ToString();

	FString AttributeString = FString::Printf(TEXT("(NeedId=\"%s\")"), *attribute);	
	if (!CurrentDefaultValue.Equals(AttributeString))
	{
		GraphPinObj->GetSchema()->TrySetDefaultValue(*GraphPinObj, AttributeString);
	}
}

FText SArcanaNeedPin::CreateNeedsComboBoxContent() const
{
	const bool bHasSelectedItem = NeedsComboBox.IsValid() && NeedsComboBox->GetSelectedItem().IsValid();

	return bHasSelectedItem ? FText::FromName(*NeedsComboBox->GetSelectedItem().Get()) : FText();
}