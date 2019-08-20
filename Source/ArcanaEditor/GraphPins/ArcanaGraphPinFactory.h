// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "ArcanaNeedPin.h"
#include "ArcanaSkillPin.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_K2.h"
#include "EdGraphUtilities.h"
#include "Needs/ArcanaNeed.h"
#include "Skills/ArcanaSkill.h"
#include "SlateBasics.h"

class FArcanaGraphPinFactory : public FGraphPanelPinFactory
{
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override
	{
		const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();
		// Check if pin is struct, and then check if that pin is of struct type we want customize
		if (InPin->PinType.PinCategory == K2Schema->PC_Struct
			&& InPin->PinType.PinSubCategoryObject == FArcanaNeed::StaticStruct())
		{
			return SNew(SArcanaNeedPin, InPin); // Return our customized pin widget
		}
		else if (InPin->PinType.PinCategory == K2Schema->PC_Struct
			&& InPin->PinType.PinSubCategoryObject == FArcanaSkill::StaticStruct())
		{
			return SNew(SArcanaSkillPin, InPin); // Return our customized pin widget
		}
		return nullptr;
	}
};