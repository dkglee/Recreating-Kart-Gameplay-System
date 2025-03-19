// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectR : ModuleRules
{
	public ProjectR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "UMG"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"MediaAssets", "SlateCore", "Slate", "EnhancedInput",
			"AdvancedWidgets", "OnlineSubsystem", "OnlineSubsystemNull",
			"OnlineSubsystemSteam", "Niagara", "AudioExtensions", 
			"MetasoundEngine", "MetasoundFrontend"
		});
		
		PublicIncludePaths.AddRange(new string[]
		{
			"ProjectR/",
			"ProjectR/KartGame/Kart/",
			"ProjectR/KartGame/Kart/Components/",
			"ProjectR/KartGame/Kart/Utils/",
			"ProjectR/KartGame/Utils/"
		});
	}
}
