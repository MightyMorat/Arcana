// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcanaAction.h"

#if WITH_EDITOR
bool UArcanaActionData::CanEditChange(const UProperty* InProperty) const
{
	if (!Super::CanEditChange(InProperty))
		return false;

	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UArcanaActionData, SkillGainRate))
	{
		return AffectedSkill.SkillId != NAME_None;
	}

	return true;
}
#endif // WITH_EDITOR