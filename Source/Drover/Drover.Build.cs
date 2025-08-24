// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Drover : ModuleRules
{
	public Drover(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Drover"
			//"Drover/Variant_Platforming",
			//"Drover/Variant_Platforming/Animation",
			//"Drover/Variant_Combat",
			//"Drover/Variant_Combat/AI",
			//"Drover/Variant_Combat/Animation",
			//"Drover/Variant_Combat/Gameplay",
			//"Drover/Variant_Combat/Interfaces",
			//"Drover/Variant_Combat/UI",
			//"Drover/Variant_SideScrolling",
			//"Drover/Variant_SideScrolling/AI",
			//"Drover/Variant_SideScrolling/Gameplay",
			//"Drover/Variant_SideScrolling/Interfaces",
			//"Drover/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
