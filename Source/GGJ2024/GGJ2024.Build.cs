// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GGJ2024 : ModuleRules
{
	public GGJ2024(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
