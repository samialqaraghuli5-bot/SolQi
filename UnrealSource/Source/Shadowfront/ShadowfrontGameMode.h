#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShadowfrontEnemy.h"
#include "ShadowfrontGameMode.generated.h"

class AStaticMeshActor;
class UAudioComponent;

UENUM()
enum class EShadowfrontDifficulty : uint8
{
    Recon,
    Operation,
    Veteran
};

UCLASS()
class SHADOWFRONT_API AShadowfrontGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AShadowfrontGameMode();
    virtual void BeginPlay() override;
    void NotifyEnemyEliminated();
    FString GetMissionText() const;
    FString GetDifficultyText() const;
    bool IsMissionComplete() const { return bMissionComplete; }
    bool IsAwaitingDifficulty() const { return bAwaitingDifficulty; }
    int32 GetRemainingHostiles() const { return RemainingHostiles; }
    int32 GetMissionStage() const { return CurrentWave; }
    void SelectDifficulty(EShadowfrontDifficulty InDifficulty);

private:
    AStaticMeshActor* AddBlock(const FVector& Location, const FVector& Scale, const FLinearColor& Color);
    void AddLight(const FVector& Location, const FLinearColor& Color, float Intensity, float Radius);
    void SpawnArena();
    void BeginFirstWave();
    void SpawnWave();
    void SpawnEnemyAt(const FVector& Location, EShadowfrontEnemyRole Role);
    void QueueNextWave();
    void CompleteMission();
    void SaveCheckpoint();
    void LoadCheckpoint();
    void ConfigureDifficulty();
    UAudioComponent* PlayMissionAudio(const TCHAR* AssetPath, float VolumeMultiplier, bool bPersist = false);
    float GetPlayerHealthMultiplier() const;
    float GetEnemyDifficultyMultiplier() const;

    int32 RemainingHostiles = 0;
    int32 CurrentWave = 0;
    bool bMissionComplete = false;
    bool bAwaitingDifficulty = false;
    bool bDifficultyFromCommandLine = false;
    EShadowfrontDifficulty Difficulty = EShadowfrontDifficulty::Operation;
    FTimerHandle MissionTimer;
    UPROPERTY(Transient) TObjectPtr<UAudioComponent> OpeningMusicComponent;
    UPROPERTY(Transient) TObjectPtr<UAudioComponent> CombatMusicComponent;
};
