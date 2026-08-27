#include "ShadowfrontGameMode.h"

#include "Components/AudioComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/LightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Engine/PointLight.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Parse.h"
#include "Shadowfront.h"
#include "ShadowfrontHUD.h"
#include "ShadowfrontPlayerController.h"
#include "ShadowfrontSaveGame.h"
#include "ShadowfrontSoldier.h"
#include "TimerManager.h"

AShadowfrontGameMode::AShadowfrontGameMode()
{
    DefaultPawnClass = AShadowfrontSoldier::StaticClass();
    HUDClass = AShadowfrontHUD::StaticClass();
    PlayerControllerClass = AShadowfrontPlayerController::StaticClass();
}

void AShadowfrontGameMode::BeginPlay()
{
    Super::BeginPlay();
    LoadCheckpoint();
    ConfigureDifficulty();
    SpawnArena();
    OpeningMusicComponent = PlayMissionAudio(TEXT("/Game/Audio/Music/shadowfront_opening_theme.shadowfront_opening_theme"), 0.34f, true);
    PlayMissionAudio(TEXT("/Game/Audio/Dialogue/shadowfront_opening_comms.shadowfront_opening_comms"), 0.88f);
    if (AShadowfrontSoldier* Soldier = Cast<AShadowfrontSoldier>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        Soldier->ConfigureDifficulty(GetPlayerHealthMultiplier());
        Soldier->SetCheckpoint(Soldier->GetActorLocation());
    }
    if (bMissionComplete)
    {
        PlayMissionAudio(TEXT("/Game/Audio/Music/shadowfront_exploration_bed.shadowfront_exploration_bed"), 0.30f);
        return;
    }
    if (CurrentWave == 0 && !bDifficultyFromCommandLine)
    {
        bAwaitingDifficulty = true;
        return;
    }
    GetWorldTimerManager().SetTimer(MissionTimer, this, CurrentWave > 0 ? &AShadowfrontGameMode::QueueNextWave : &AShadowfrontGameMode::BeginFirstWave, 2.0f, false);
}

UAudioComponent* AShadowfrontGameMode::PlayMissionAudio(const TCHAR* AssetPath, float VolumeMultiplier, bool bPersist)
{
    if (USoundBase* Sound = LoadObject<USoundBase>(nullptr, AssetPath))
    {
        return UGameplayStatics::SpawnSound2D(this, Sound, VolumeMultiplier, 1.0f, 0.0f, nullptr, bPersist, false);
    }
    UE_LOG(LogShadowfront, Verbose, TEXT("Mission audio asset is not available yet: %s"), AssetPath);
    return nullptr;
}

AStaticMeshActor* AShadowfrontGameMode::AddBlock(const FVector& Location, const FVector& Scale, const FLinearColor& Color)
{
    AStaticMeshActor* Block = GetWorld()->SpawnActor<AStaticMeshActor>(Location, FRotator::ZeroRotator);
    if (!Block) { return nullptr; }
    UStaticMeshComponent* Mesh = Block->GetStaticMeshComponent();
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"));
    if (CubeMesh.Succeeded()) { Mesh->SetStaticMesh(CubeMesh.Object); }
    Mesh->SetWorldScale3D(Scale);
    if (BaseMaterial.Succeeded())
    {
        UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial.Object, this);
        DynamicMaterial->SetVectorParameterValue(TEXT("Color"), Color);
        Mesh->SetMaterial(0, DynamicMaterial);
    }
    return Block;
}

void AShadowfrontGameMode::AddLight(const FVector& Location, const FLinearColor& Color, float Intensity, float Radius)
{
    APointLight* Light = GetWorld()->SpawnActor<APointLight>(Location, FRotator::ZeroRotator);
    if (Light)
    {
        Light->GetLightComponent()->SetLightColor(Color);
        Light->GetLightComponent()->SetIntensity(Intensity);
        if (UPointLightComponent* PointLight = Cast<UPointLightComponent>(Light->GetLightComponent())) { PointLight->SetAttenuationRadius(Radius); }
        Light->GetLightComponent()->SetCastShadows(false);
    }
}

void AShadowfrontGameMode::SpawnArena()
{
    const FLinearColor Concrete(0.10f, 0.13f, 0.16f);
    const FLinearColor Steel(0.035f, 0.06f, 0.08f);
    const FLinearColor Barrier(0.28f, 0.29f, 0.28f);
    const FLinearColor DawnBlue(0.16f, 0.28f, 0.48f);
    const FLinearColor WarmLamp(1.0f, 0.42f, 0.12f);
    const FLinearColor RadarGreen(0.08f, 0.63f, 0.36f);

    AddBlock(FVector(2150.0f, 0.0f, -118.0f), FVector(98.0f, 30.0f, 0.20f), Concrete);
    AddBlock(FVector(3200.0f, -2850.0f, 340.0f), FVector(31.0f, 0.25f, 5.0f), Steel);
    AddBlock(FVector(3200.0f, 2850.0f, 340.0f), FVector(31.0f, 0.25f, 5.0f), Steel);
    AddBlock(FVector(1240.0f, -780.0f, 130.0f), FVector(6.2f, 3.6f, 2.4f), Steel);
    AddBlock(FVector(2180.0f, 780.0f, 130.0f), FVector(6.2f, 3.6f, 2.4f), Steel);
    AddBlock(FVector(3150.0f, -840.0f, 130.0f), FVector(6.2f, 3.6f, 2.4f), Steel);
    AddBlock(FVector(3880.0f, 840.0f, 130.0f), FVector(6.2f, 3.6f, 2.4f), Steel);
    AddBlock(FVector(1730.0f, -260.0f, 22.0f), FVector(1.25f, 2.0f, 0.7f), Barrier);
    AddBlock(FVector(1900.0f, 320.0f, 22.0f), FVector(1.25f, 2.0f, 0.7f), Barrier);
    AddBlock(FVector(2960.0f, 240.0f, 22.0f), FVector(1.25f, 2.0f, 0.7f), Barrier);
    AddBlock(FVector(3640.0f, -300.0f, 22.0f), FVector(1.25f, 2.0f, 0.7f), Barrier);
    AddBlock(FVector(4850.0f, 0.0f, 300.0f), FVector(0.45f, 0.45f, 6.3f), Steel);
    AddBlock(FVector(4850.0f, 0.0f, 910.0f), FVector(5.0f, 0.16f, 0.16f), Steel);

    AddLight(FVector(880.0f, -1050.0f, 430.0f), DawnBlue, 15000.0f, 3200.0f);
    AddLight(FVector(1750.0f, 980.0f, 300.0f), WarmLamp, 5500.0f, 1100.0f);
    AddLight(FVector(2500.0f, -920.0f, 300.0f), WarmLamp, 5500.0f, 1100.0f);
    AddLight(FVector(3580.0f, 900.0f, 300.0f), WarmLamp, 5500.0f, 1100.0f);
    AddLight(FVector(4850.0f, 0.0f, 720.0f), RadarGreen, 7800.0f, 1750.0f);

    AExponentialHeightFog* Fog = GetWorld()->SpawnActor<AExponentialHeightFog>(FVector(0.0f, 0.0f, -90.0f), FRotator::ZeroRotator);
    if (Fog)
    {
        Fog->GetComponent()->SetFogDensity(0.014f);
        Fog->GetComponent()->SetFogInscatteringColor(FLinearColor(0.05f, 0.11f, 0.19f));
    }
}

void AShadowfrontGameMode::ConfigureDifficulty()
{
    FString DifficultyArgument;
    if (FParse::Value(FCommandLine::Get(), TEXT("ShadowfrontDifficulty="), DifficultyArgument))
    {
        bDifficultyFromCommandLine = true;
        DifficultyArgument = DifficultyArgument.ToUpper();
        if (DifficultyArgument == TEXT("RECON")) { Difficulty = EShadowfrontDifficulty::Recon; }
        else if (DifficultyArgument == TEXT("VETERAN")) { Difficulty = EShadowfrontDifficulty::Veteran; }
        else { Difficulty = EShadowfrontDifficulty::Operation; }
    }
}

void AShadowfrontGameMode::SelectDifficulty(EShadowfrontDifficulty InDifficulty)
{
    if (!bAwaitingDifficulty) { return; }
    Difficulty = InDifficulty;
    bAwaitingDifficulty = false;
    bDifficultyFromCommandLine = false;
    if (AShadowfrontSoldier* Soldier = Cast<AShadowfrontSoldier>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        Soldier->ConfigureDifficulty(GetPlayerHealthMultiplier());
        Soldier->SetCheckpoint(Soldier->GetActorLocation());
    }
    GetWorldTimerManager().SetTimer(MissionTimer, this, &AShadowfrontGameMode::BeginFirstWave, 0.85f, false);
}

float AShadowfrontGameMode::GetPlayerHealthMultiplier() const
{
    switch (Difficulty)
    {
        case EShadowfrontDifficulty::Recon: return 1.30f;
        case EShadowfrontDifficulty::Veteran: return 0.82f;
        default: return 1.0f;
    }
}

float AShadowfrontGameMode::GetEnemyDifficultyMultiplier() const
{
    switch (Difficulty)
    {
        case EShadowfrontDifficulty::Recon: return 0.75f;
        case EShadowfrontDifficulty::Veteran: return 1.28f;
        default: return 1.0f;
    }
}

void AShadowfrontGameMode::BeginFirstWave()
{
    CurrentWave = 1;
    if (OpeningMusicComponent) { OpeningMusicComponent->FadeOut(1.25f, 0.0f); }
    CombatMusicComponent = PlayMissionAudio(TEXT("/Game/Audio/Music/shadowfront_combat_loop.shadowfront_combat_loop"), 0.42f, true);
    PlayMissionAudio(TEXT("/Game/Audio/Dialogue/shadowfront_marwan_barks.shadowfront_marwan_barks"), 0.82f);
    SpawnWave();
}

void AShadowfrontGameMode::SpawnWave()
{
    TArray<FVector> SpawnPoints;
    TArray<EShadowfrontEnemyRole> Roles;
    if (CurrentWave == 1)
    {
        SpawnPoints = { FVector(2520.0f, 0.0f, 30.0f), FVector(2780.0f, -520.0f, 30.0f), FVector(2950.0f, 610.0f, 30.0f) };
        Roles = { EShadowfrontEnemyRole::Rifleman, EShadowfrontEnemyRole::Rifleman, EShadowfrontEnemyRole::Spotter };
    }
    else if (CurrentWave == 2)
    {
        SpawnPoints = { FVector(3260.0f, -820.0f, 30.0f), FVector(3380.0f, 780.0f, 30.0f), FVector(3650.0f, -210.0f, 30.0f), FVector(3740.0f, 470.0f, 30.0f) };
        Roles = { EShadowfrontEnemyRole::Breacher, EShadowfrontEnemyRole::Rifleman, EShadowfrontEnemyRole::DroneOperator, EShadowfrontEnemyRole::Rifleman };
    }
    else
    {
        SpawnPoints = { FVector(4200.0f, -880.0f, 30.0f), FVector(4320.0f, 840.0f, 30.0f), FVector(4540.0f, -310.0f, 30.0f), FVector(4630.0f, 420.0f, 30.0f), FVector(4750.0f, 0.0f, 30.0f) };
        Roles = { EShadowfrontEnemyRole::Breacher, EShadowfrontEnemyRole::Spotter, EShadowfrontEnemyRole::Rifleman, EShadowfrontEnemyRole::DroneOperator, EShadowfrontEnemyRole::Breacher };
    }

    RemainingHostiles = SpawnPoints.Num();
    for (int32 Index = 0; Index < SpawnPoints.Num(); ++Index) { SpawnEnemyAt(SpawnPoints[Index], Roles[Index]); }
}

void AShadowfrontGameMode::SpawnEnemyAt(const FVector& Location, EShadowfrontEnemyRole Role)
{
    const FTransform SpawnTransform(FRotator(0.0f, 180.0f, 0.0f), Location);
    AShadowfrontEnemy* Enemy = GetWorld()->SpawnActorDeferred<AShadowfrontEnemy>(AShadowfrontEnemy::StaticClass(), SpawnTransform);
    if (Enemy)
    {
        Enemy->ConfigureEnemy(Role, GetEnemyDifficultyMultiplier());
        Enemy->FinishSpawning(SpawnTransform);
    }
}

void AShadowfrontGameMode::NotifyEnemyEliminated()
{
    RemainingHostiles = FMath::Max(0, RemainingHostiles - 1);
    if (RemainingHostiles == 0 && !bMissionComplete)
    {
        SaveCheckpoint();
        if (CurrentWave < 3)
        {
            ++CurrentWave;
            SaveCheckpoint();
            GetWorldTimerManager().SetTimer(MissionTimer, this, &AShadowfrontGameMode::QueueNextWave, 3.0f, false);
        }
        else { GetWorldTimerManager().SetTimer(MissionTimer, this, &AShadowfrontGameMode::CompleteMission, 2.0f, false); }
    }
}

void AShadowfrontGameMode::QueueNextWave()
{
    SpawnWave();
}

void AShadowfrontGameMode::CompleteMission()
{
    bMissionComplete = true;
    if (CombatMusicComponent) { CombatMusicComponent->FadeOut(1.75f, 0.0f); }
    PlayMissionAudio(TEXT("/Game/Audio/Dialogue/shadowfront_silent_warden.shadowfront_silent_warden"), 0.86f);
    PlayMissionAudio(TEXT("/Game/Audio/Music/shadowfront_exploration_bed.shadowfront_exploration_bed"), 0.30f);
    SaveCheckpoint();
}

void AShadowfrontGameMode::SaveCheckpoint()
{
    UShadowfrontSaveGame* SaveGame = Cast<UShadowfrontSaveGame>(UGameplayStatics::CreateSaveGameObject(UShadowfrontSaveGame::StaticClass()));
    if (!SaveGame) { return; }
    SaveGame->CurrentWave = CurrentWave;
    SaveGame->DifficultyName = GetDifficultyText();
    SaveGame->bMissionComplete = bMissionComplete;
    UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("ShadowfrontCampaign"), 0);
}

void AShadowfrontGameMode::LoadCheckpoint()
{
    if (!UGameplayStatics::DoesSaveGameExist(TEXT("ShadowfrontCampaign"), 0)) { return; }
    const UShadowfrontSaveGame* SaveGame = Cast<UShadowfrontSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShadowfrontCampaign"), 0));
    if (!SaveGame) { return; }

    CurrentWave = FMath::Clamp(SaveGame->CurrentWave, 0, 3);
    bMissionComplete = SaveGame->bMissionComplete;
    const FString SavedDifficulty = SaveGame->DifficultyName.ToUpper();
    if (SavedDifficulty == TEXT("RECON")) { Difficulty = EShadowfrontDifficulty::Recon; }
    else if (SavedDifficulty == TEXT("VETERAN")) { Difficulty = EShadowfrontDifficulty::Veteran; }
    else { Difficulty = EShadowfrontDifficulty::Operation; }

    if (bMissionComplete) { CurrentWave = 3; }
}

FString AShadowfrontGameMode::GetMissionText() const
{
    if (bMissionComplete) { return TEXT("ATLAS RELAY SECURED // EXFIL READY"); }
    if (CurrentWave == 0) { return TEXT("PIER SEVEN // ESTABLISH UPLINK"); }
    return FString::Printf(TEXT("SECURE THE RELAY // WAVE %d // %d HOSTILES"), CurrentWave, RemainingHostiles);
}

FString AShadowfrontGameMode::GetDifficultyText() const
{
    switch (Difficulty)
    {
        case EShadowfrontDifficulty::Recon: return TEXT("RECON");
        case EShadowfrontDifficulty::Veteran: return TEXT("VETERAN");
        default: return TEXT("OPERATION");
    }
}
