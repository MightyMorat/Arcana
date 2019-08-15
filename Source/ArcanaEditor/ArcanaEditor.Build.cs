// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ArcanaEditor : ModuleRules
{
	public ArcanaEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
            "Arcana",
            "Core",
            "CoreUObject",
            "Engine",
            "UnrealEd",
            "PropertyEditor",
            "Slate",
            "SlateCore",
            "EditorStyle",
            "InputCore",
            "GraphEditor",
            "BlueprintGraph"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
