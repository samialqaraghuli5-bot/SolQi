// Shadowfront visual contract: a compact original mobile combat mission designed for scalable pre-dawn atmosphere.
using UnrealBuildTool;

public class ShadowfrontTarget : TargetRules
{
    public ShadowfrontTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        BuildEnvironment = TargetBuildEnvironment.Unique;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("Shadowfront");
    }
}
