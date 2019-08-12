// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ArcanaEditor : ModuleRules
{
	public ArcanaEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Arcana", "Core", "CoreUObject", "Engine" });

		PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
	}
}
