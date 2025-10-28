// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DroverEditorTarget : TargetRules
{
	public DroverEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("Drover");
		ExtraModuleNames.Add("DroverAnimation");
        ExtraModuleNames.Add("DroverAnimationEditor");
    }
}
