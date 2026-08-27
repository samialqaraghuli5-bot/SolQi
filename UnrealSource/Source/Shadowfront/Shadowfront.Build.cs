// Shadowfront visual contract: lightweight native systems supporting a readable cinematic mobile shooter.
using UnrealBuildTool;

public class Shadowfront : ModuleRules
{
    public Shadowfront(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "ApplicationCore" });
    }
}
