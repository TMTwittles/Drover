// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DroverAnimation : ModuleRules
{
	public DroverAnimation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"AnimGraphRuntime"
		});
	}
}
