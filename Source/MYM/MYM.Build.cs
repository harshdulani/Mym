// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MYM : ModuleRules
{
	public MYM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore", 
			"EnhancedInput",
			"GameplayTags",
			
		});
	}
}
