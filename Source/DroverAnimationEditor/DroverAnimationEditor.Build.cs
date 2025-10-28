// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DroverAnimationEditor : ModuleRules
{
	public DroverAnimationEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		OverridePackageType = PackageOverrideType.GameUncookedOnly;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"AnimGraph",
            "UnrealEd",
			"BlueprintGraph",
			"DroverAnimation"
        });
	}
}
