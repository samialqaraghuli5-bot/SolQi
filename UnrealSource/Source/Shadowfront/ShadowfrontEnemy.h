#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShadowfrontEnemy.generated.h"

class UPointLightComponent;
class UStaticMeshComponent;

UENUM()
enum class EShadowfrontEnemyRole : uint8
{
    Rifleman,
    Breacher,
    Spotter,
    DroneOperator
};

UCLASS()
class SHADOWFRONT_API AShadowfrontEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    AShadowfrontEnemy();
    virtual void Tick(float DeltaSeconds) override;
    virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    void ConfigureEnemy(EShadowfrontEnemyRole InRole, float InDifficultyMultiplier);
    bool IsNeutralized() const { return bNeutralized; }

protected:
    virtual void BeginPlay() override;

private:
    void TryFire();
    void Neutralize();
    bool HasClearShot(const APawn* PlayerPawn) const;
    void ApplyRoleVisuals();

    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Components") TObjectPtr<UStaticMeshComponent> TorsoMesh;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Components") TObjectPtr<UStaticMeshComponent> HeadMesh;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Components") TObjectPtr<UPointLightComponent> MarkerLight;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Combat") EShadowfrontEnemyRole EnemyRole = EShadowfrontEnemyRole::Rifleman;
    UPROPERTY(EditDefaultsOnly, Category = "Shadowfront|Combat") float BaseHealth = 100.0f;

    float Health = 100.0f;
    float DamagePerShot = 7.0f;
    float AttackRange = 2300.0f;
    float PreferredRange = 1500.0f;
    float FireInterval = 1.35f;
    float AdvanceSpeed = 115.0f;
    bool bNeutralized = false;
    FTimerHandle FireTimer;
};
