// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class SpartaGameJam_2 : ModuleRules
{
	public SpartaGameJam_2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput",
			"PaperZD",
			"Niagara"
		});
		
        PublicIncludePaths.AddRange(new string[] {
            "SpartaGameJam_2",
            Path.Combine(ModuleDirectory, "Manager"),
            Path.Combine(ModuleDirectory, "Yut"),
            Path.Combine(ModuleDirectory, "Core"),
            Path.Combine(ModuleDirectory, "Character"),
            Path.Combine(ModuleDirectory, "Enum"),
            Path.Combine(ModuleDirectory, "SubSystem"),
            Path.Combine(ModuleDirectory, "TableRowBase"),
        });
		
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
