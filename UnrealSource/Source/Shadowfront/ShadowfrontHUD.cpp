#include "ShadowfrontHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "ShadowfrontGameMode.h"
#include "ShadowfrontSoldier.h"

void AShadowfrontHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas) { return; }
    AShadowfrontSoldier* Soldier = Cast<AShadowfrontSoldier>(GetOwningPawn());
    AShadowfrontGameMode* GameMode = GetWorld() ? Cast<AShadowfrontGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
    const float Width = Canvas->ClipX;
    const float Height = Canvas->ClipY;
    const FLinearColor RadarGreen(0.66f, 0.85f, 0.63f);
    const FLinearColor DangerOrange(1.0f, 0.25f, 0.06f);
    const FLinearColor SoftWhite(0.88f, 0.94f, 0.92f);
    const FLinearColor DimWhite(0.60f, 0.68f, 0.68f);
    UFont* SmallFont = GEngine ? GEngine->GetSmallFont() : nullptr;
    UFont* LargeFont = GEngine ? GEngine->GetLargeFont() : nullptr;

    DrawText(TEXT("SHADOWFRONT // ASH LINE"), SoftWhite, 36.0f, 22.0f, SmallFont, 0.80f, false);
    DrawText(GameMode ? GameMode->GetMissionText() : TEXT("ESTABLISHING UPLINK"), RadarGreen, 36.0f, 48.0f, SmallFont, 1.08f, false);
    DrawText(GameMode ? FString::Printf(TEXT("DIFFICULTY // %s"), *GameMode->GetDifficultyText()) : TEXT("DIFFICULTY // OPERATION"), DimWhite, 36.0f, 74.0f, SmallFont, 0.72f, false);

    const float HealthPercent = Soldier ? Soldier->GetHealthPercent() : 1.0f;
    DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.50f), 36.0f, 100.0f, 260.0f, 14.0f);
    DrawRect(HealthPercent < 0.30f ? DangerOrange : RadarGreen, 36.0f, 100.0f, 260.0f * HealthPercent, 14.0f);
    DrawText(TEXT("VITAL"), SoftWhite, 304.0f, 94.0f, SmallFont, 0.88f, false);

    const FString Ammo = FString::Printf(TEXT("%02d / %03d"), Soldier ? Soldier->GetMagazineAmmo() : 0, Soldier ? Soldier->GetReserveAmmo() : 0);
    DrawText(Ammo, SoftWhite, Width - 255.0f, Height - 142.0f, LargeFont, 1.35f, false);
    const FString Status = Soldier && Soldier->IsReloading() ? TEXT("RELOADING") : Soldier && Soldier->IsSprinting() ? TEXT("SPRINTING") : TEXT("TAP RIGHT // FIRE");
    DrawText(Status, RadarGreen, Width - 255.0f, Height - 88.0f, SmallFont, 0.90f, false);
    if (GameMode)
    {
        DrawText(FString::Printf(TEXT("WAVE %d / 3 // HOSTILES %02d"), GameMode->GetMissionStage(), GameMode->GetRemainingHostiles()), DimWhite, Width - 305.0f, 42.0f, SmallFont, 0.76f, false);
    }

    const float CenterX = Width * 0.5f;
    const float CenterY = Height * 0.5f;
    DrawLine(CenterX - 16.0f, CenterY, CenterX - 5.0f, CenterY, RadarGreen, 2.0f);
    DrawLine(CenterX + 5.0f, CenterY, CenterX + 16.0f, CenterY, RadarGreen, 2.0f);
    DrawLine(CenterX, CenterY - 16.0f, CenterX, CenterY - 5.0f, RadarGreen, 2.0f);
    DrawLine(CenterX, CenterY + 5.0f, CenterX, CenterY + 16.0f, RadarGreen, 2.0f);
    DrawLine(28.0f, Height - 112.0f, 130.0f, Height - 112.0f, FLinearColor(1.0f, 1.0f, 1.0f, 0.28f), 1.0f);
    DrawText(TEXT("MOVE"), SoftWhite, 44.0f, Height - 102.0f, SmallFont, 0.85f, false);
}
