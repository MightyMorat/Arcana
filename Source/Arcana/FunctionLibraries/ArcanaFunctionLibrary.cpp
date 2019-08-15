// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaFunctionLibrary.h"

#include "Settings/ArcanaSettings.h"

void UArcanaFunctionLibrary::GetNeedDefinition(const FArcanaNeed& Need, bool& bFound, FArcanaNeedDefinition& NeedDefinition)
{
	bFound = false;
	NeedDefinition = FArcanaNeedDefinition();

	UDataTable* NeedsDataTable = UArcanaSettings::Get()->NeedsDataTable.LoadSynchronous();
	if (NeedsDataTable)
	{
		FArcanaNeedDefinition* Definition = NeedsDataTable->FindRow<FArcanaNeedDefinition>(Need.NeedId, FString(""), false);
		if (Definition)
		{
			bFound = true;
			NeedDefinition = *Definition;
		}
	}
}