// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcanaTime.h"

#include "Settings/ArcanaSettings.h"

void FArcanaTimeInfo::InitFromTotalTimeHours(float TotalTimeHours)
{
	const float TotalTimeDays = TotalTimeHours / 24.0f;
	CurrentDay = FMath::FloorToInt(TotalTimeDays);
	ProgressThroughDay = FMath::Clamp(TotalTimeDays - CurrentDay, 0.0f, 1.0f);

	const float TimeHours = FMath::Fmod(TotalTimeHours, 24.0f);
	CurrentHour = FMath::FloorToInt(TimeHours);
	ProgressThroughHour = FMath::Clamp(TimeHours - CurrentHour, 0.0f, 1.0f);

	UArcanaSettings* Settings = UArcanaSettings::Get();
	if (CurrentHour < Settings->NightEndHour)
	{
		TimeOfDay = EArcanaTimeOfDay::Night;
	}
	else if (CurrentHour < Settings->DayStartHour)
	{
		TimeOfDay = EArcanaTimeOfDay::Dawn;
	}
	else if (CurrentHour < Settings->DayEndHour)
	{
		TimeOfDay = EArcanaTimeOfDay::Day;
	}
	else if (CurrentHour < Settings->NightStartHour)
	{
		TimeOfDay = EArcanaTimeOfDay::Dusk;
	}
	else
	{
		TimeOfDay = EArcanaTimeOfDay::Night;
	}
}