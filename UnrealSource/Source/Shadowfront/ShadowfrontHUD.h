// Shadowfront visual contract: radar-green data at the screen edges, a thin crosshair, and orange reserved for danger.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShadowfrontHUD.generated.h"

UCLASS()
class SHADOWFRONT_API AShadowfrontHUD : public AHUD
{
    GENERATED_BODY()
public:
    virtual void DrawHUD() override;
};
