// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "Widgets/Input/SComboBox.h"

class FArcanaNeedCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	TSharedRef<SWidget> OnGenerateNeedsComboBox(TSharedPtr<FName> InItem);
	void OnSelectionChanged(TSharedPtr<FName> InItem, ESelectInfo::Type SelectInfo);
	FText CreateNeedsComboBoxContent() const;

	TSharedPtr<SComboBox<TSharedPtr<FName>>> NeedsComboBox;
	TSharedPtr<IPropertyHandle> NeedIdPropertyHandle;
	TArray< TSharedPtr<FName>> NeedOptions;
};
