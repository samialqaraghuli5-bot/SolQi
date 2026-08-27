// Shadowfront visual contract: editor target for building the original Android combat prototype.
using UnrealBuildTool;

public class ShadowfrontEditorTarget : TargetRules
{
    public ShadowfrontEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        BuildEnvironment = TargetBuildEnvironment.Unique;
        DefaultBuildSettings = BuildSettingsVersion.V7;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
        ExtraModuleNames.Add("Shadowfront");
    }
}
