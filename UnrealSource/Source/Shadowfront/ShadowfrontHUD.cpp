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

    if (GameMode && GameMode->IsAwaitingDifficulty())
    {
        const float PanelWidth = FMath::Min(980.0f, Width - 72.0f);
        const float PanelX = (Width - PanelWidth) * 0.5f;
        const float PanelY = Height * 0.24f;
        const float CardGap = 14.0f;
        const float CardWidth = (PanelWidth - (CardGap * 2.0f)) / 3.0f;
        const float CardHeight = 150.0f;
        const FLinearColor Panel(0.015f, 0.030f, 0.045f, 0.92f);
        const FLinearColor Recon(0.08f, 0.34f, 0.26f, 0.96f);
        const FLinearColor Operation(0.16f, 0.20f, 0.23f, 0.96f);
        const FLinearColor Veteran(0.40f, 0.12f, 0.05f, 0.96f);

        DrawRect(Panel, PanelX, PanelY, PanelWidth, 260.0f);
        DrawText(TEXT("SHADOWFRONT // ASH LINE"), SoftWhite, PanelX + 32.0f, PanelY + 25.0f, LargeFont, 1.18f, false);
        DrawText(TEXT("SELECT FIELD DIRECTIVE"), RadarGreen, PanelX + 34.0f, PanelY + 79.0f, SmallFont, 0.90f, false);

        DrawRect(Recon, PanelX, PanelY + 110.0f, CardWidth, CardHeight);
        DrawRect(Operation, PanelX + CardWidth + CardGap, PanelY + 110.0f, CardWidth, CardHeight);
        DrawRect(Veteran, PanelX + (CardWidth + CardGap) * 2.0f, PanelY + 110.0f, CardWidth, CardHeight);
        DrawText(TEXT("1 // RECON"), SoftWhite, PanelX + 20.0f, PanelY + 135.0f, SmallFont, 1.10f, false);
        DrawText(TEXT("MORE ARMOR"), SoftWhite, PanelX + 20.0f, PanelY + 181.0f, SmallFont, 0.74f, false);
        DrawText(TEXT("2 // OPERATION"), SoftWhite, PanelX + CardWidth + CardGap + 20.0f, PanelY + 135.0f, SmallFont, 1.10f, false);
        DrawText(TEXT("STANDARD ORDER"), SoftWhite, PanelX + CardWidth + CardGap + 20.0f, PanelY + 181.0f, SmallFont, 0.74f, false);
        DrawText(TEXT("3 // VETERAN"), SoftWhite, PanelX + (CardWidth + CardGap) * 2.0f + 20.0f, PanelY + 135.0f, SmallFont, 1.10f, false);
        DrawText(TEXT("HARD CONTACT"), SoftWhite, PanelX + (CardWidth + CardGap) * 2.0f + 20.0f, PanelY + 181.0f, SmallFont, 0.74f, false);
        DrawText(TEXT("TAP A CARD TO DEPLOY"), DimWhite, PanelX + 34.0f, PanelY + 287.0f, SmallFont, 0.76f, false);
        return;
    }

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

    const float TouchSize = FMath::Clamp(Height * 0.13f, 72.0f, 118.0f);
    const float FireX = Width - TouchSize - 32.0f;
    const float FireY = Height - TouchSize - 30.0f;
    DrawRect(FLinearColor(0.48f, 0.10f, 0.04f, 0.72f), FireX, FireY, TouchSize, TouchSize);
    DrawText(TEXT("FIRE"), SoftWhite, FireX + 21.0f, FireY + (TouchSize * 0.42f), SmallFont, 0.92f, false);
    DrawRect(FLinearColor(0.08f, 0.19f, 0.18f, 0.70f), FireX - TouchSize - 18.0f, FireY + (TouchSize * 0.28f), TouchSize * 0.78f, TouchSize * 0.58f);
    DrawText(TEXT("SPRINT"), SoftWhite, FireX - TouchSize - 10.0f, FireY + (TouchSize * 0.56f), SmallFont, 0.68f, false);
    DrawRect(FLinearColor(0.10f, 0.16f, 0.22f, 0.72f), FireX, FireY - (TouchSize * 0.62f) - 14.0f, TouchSize, TouchSize * 0.50f);
    DrawText(TEXT("RELOAD"), SoftWhite, FireX + 12.0f, FireY - (TouchSize * 0.33f) - 14.0f, SmallFont, 0.66f, false);
}
