// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaNeedCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "ArcanaNeedCustomization"

TSharedRef<IPropertyTypeCustomization> FArcanaNeedCustomization::MakeInstance()
{
	return MakeShareable(new FArcanaNeedCustomization());
}

void FArcanaNeedCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		const TSharedRef< IPropertyHandle > ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();

		if (ChildHandle->GetProperty()->GetName() == TEXT("NeedId"))
		{
			NeedIdPropertyHandle = ChildHandle;
		}
	}

	check(NeedIdPropertyHandle.IsValid());

	NeedOptions.Empty();
	NeedOptions.Add(MakeShareable(new FName(TEXT("None"))));
	NeedOptions.Add(MakeShareable(new FName(TEXT("Health"))));
	NeedOptions.Add(MakeShareable(new FName(TEXT("Energy"))));
	NeedOptions.Add(MakeShareable(new FName(TEXT("Sanity"))));

	FName CurrentlySelectedNeed;
	NeedIdPropertyHandle->GetValue(CurrentlySelectedNeed);
	int32 CurrentlySelectedIndex = 0;
	for (int32 i = 0; i < NeedOptions.Num(); ++i)
	{
		if (*NeedOptions[i].Get() == CurrentlySelectedNeed)
		{
			CurrentlySelectedIndex = i;
			break;
		}
	}	

	HeaderRow.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		[
			SAssignNew(NeedsComboBox, SComboBox<TSharedPtr<FName>>)
			.OptionsSource(&NeedOptions)
			.InitiallySelectedItem(NeedOptions[CurrentlySelectedIndex])
			.OnGenerateWidget(this, &FArcanaNeedCustomization::OnGenerateNeedsComboBox)
			.OnSelectionChanged(this, &FArcanaNeedCustomization::OnSelectionChanged)
			.Content()
			[
				SNew(STextBlock)
				.Text(this, &FArcanaNeedCustomization::CreateNeedsComboBoxContent)
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
		];
}

TSharedRef<SWidget> FArcanaNeedCustomization::OnGenerateNeedsComboBox(TSharedPtr<FName> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromName(*InItem.Get()));
}

void FArcanaNeedCustomization::OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type SelectInfo)
{
	NeedIdPropertyHandle->SetValue(*InItem.Get());
}

FText FArcanaNeedCustomization::CreateNeedsComboBoxContent() const
{
	const bool bHasSelectedItem = NeedsComboBox.IsValid() && NeedsComboBox->GetSelectedItem().IsValid();

	return bHasSelectedItem ? FText::FromName(*NeedsComboBox->GetSelectedItem().Get()) : FText();
}

void FArcanaNeedCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	//Create further customization here
}

#undef LOCTEXT_NAMESPACE