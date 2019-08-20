// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "Widgets/Input/SComboBox.h"

class UDataTable;

class FArcanaTableRowCustomization : public IPropertyTypeCustomization
{
public:
	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

protected:
	virtual FString GetPropertyName() const = 0;
	virtual UDataTable* GetDataTable() const = 0;

private:
	TSharedRef<SWidget> OnGenerateComboBox(TSharedPtr<FName> InItem);
	void OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type SelectInfo);
	FText CreateComboBoxContent() const;
	void OnPropertyValueChanged();

	TSharedPtr<SComboBox<TSharedPtr<FName>>> ComboBox;
	TSharedPtr<IPropertyHandle> PropertyHandle;
	TArray< TSharedPtr<FName>> Options;
};
