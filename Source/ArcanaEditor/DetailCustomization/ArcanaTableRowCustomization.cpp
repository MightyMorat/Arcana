// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaTableRowCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "Settings/ArcanaSettings.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "ArcanaTableRowCustomization"

void FArcanaTableRowCustomization::CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		const TSharedRef< IPropertyHandle > ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();

		if (ChildHandle->GetProperty()->GetName() == GetPropertyName())
		{
			PropertyHandle = ChildHandle;
		}
	}

	check(PropertyHandle.IsValid());

	StructPropertyHandle->SetOnPropertyResetToDefault(FSimpleDelegate::CreateSP(this, &FArcanaTableRowCustomization::OnPropertyValueChanged));

	TSharedPtr<IPropertyHandle> ParentProperty = StructPropertyHandle->GetParentHandle();
	while (ParentProperty.IsValid())
	{
		ParentProperty->SetOnPropertyResetToDefault(FSimpleDelegate::CreateSP(this, &FArcanaTableRowCustomization::OnPropertyValueChanged));
		ParentProperty = ParentProperty->GetParentHandle();
	}

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

	FName CurrentlySelectedOption;
	PropertyHandle->GetValue(CurrentlySelectedOption);
	int32 CurrentlySelectedIndex = 0;
	for (int32 i = 0; i < Options.Num(); ++i)
	{
		if (*Options[i].Get() == CurrentlySelectedOption)
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
			SAssignNew(ComboBox, SComboBox<TSharedPtr<FName>>)
			.OptionsSource(&Options)
			.InitiallySelectedItem(Options[CurrentlySelectedIndex])
			.OnGenerateWidget(this, &FArcanaTableRowCustomization::OnGenerateComboBox)
			.OnSelectionChanged(this, &FArcanaTableRowCustomization::OnSelectionChanged)
			.Content()
			[
				SNew(STextBlock)
				.Text(this, &FArcanaTableRowCustomization::CreateComboBoxContent)
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
		];
}

TSharedRef<SWidget> FArcanaTableRowCustomization::OnGenerateComboBox(TSharedPtr<FName> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromName(*InItem.Get()));
}

void FArcanaTableRowCustomization::OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type SelectInfo)
{
	PropertyHandle->SetValue(*InItem.Get());
}

void FArcanaTableRowCustomization::OnPropertyValueChanged()
{
	FName CurrentlySelectedOption;
	PropertyHandle->GetValue(CurrentlySelectedOption);
	int32 CurrentlySelectedIndex = 0;
	for (int32 i = 0; i < Options.Num(); ++i)
	{
		if (*Options[i].Get() == CurrentlySelectedOption)
		{
			CurrentlySelectedIndex = i;
			break;
		}
	}

	ComboBox->SetSelectedItem(Options[CurrentlySelectedIndex]);
}

FText FArcanaTableRowCustomization::CreateComboBoxContent() const
{
	const bool bHasSelectedItem = ComboBox.IsValid() && ComboBox->GetSelectedItem().IsValid();

	return bHasSelectedItem ? FText::FromName(*ComboBox->GetSelectedItem().Get()) : FText();
}

void FArcanaTableRowCustomization::CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	//Create further customization here
}

#undef LOCTEXT_NAMESPACE