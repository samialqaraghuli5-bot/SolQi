#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShadowfrontSoldier.generated.h"

class UCameraComponent;
class UPointLightComponent;
class UStaticMeshComponent;

UCLASS()
class SHADOWFRONT_API AShadowfrontSoldier : public ACharacter
{
    GENERATED_BODY()

public:
    AShadowfrontSoldier();
    virtual void Tick(float DeltaSeconds) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    float GetHealthPercent() const;
    int32 GetMagazineAmmo() const { return MagazineAmmo; }
    int32 GetReserveAmmo() const { return ReserveAmmo; }
    bool IsReloading() const { return bReloading; }
    bool IsSprinting() const { return bSprinting; }
    void ApplyCombatDamage(float DamageAmount);
    void ConfigureDifficulty(float HealthMultiplier);
    void SetCheckpoint(const FVector& NewCheckpoint);

protected:
    virtual void BeginPlay() override;

private:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Fire();
    void BeginReload();
    void FinishReload();
    void FinishRecoil();
    void FinishMuzzleFlash();
    void StartSprint();
    void StopSprint();
    void OnTouchBegin(ETouchIndex::Type FingerIndex, FVector Location);
    void RestoreAfterDefeat();

    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Components") TObjectPtr<UCameraComponent> FirstPersonCamera;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Components") TObjectPtr<UStaticMeshComponent> WeaponMesh;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Components") TObjectPtr<UPointLightComponent> MuzzleLight;

    UPROPERTY(EditDefaultsOnly, Category = "Shadowfront|Combat") float BaseMaxHealth = 100.0f;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Combat") float MaxHealth = 100.0f;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Combat") float Health = 100.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Shadowfront|Combat") int32 MagazineCapacity = 30;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Combat") int32 MagazineAmmo = 30;
    UPROPERTY(VisibleAnywhere, Category = "Shadowfront|Combat") int32 ReserveAmmo = 120;
    UPROPERTY(EditDefaultsOnly, Category = "Shadowfront|Movement") float WalkSpeed = 520.0f;
    UPROPERTY(EditDefaultsOnly, Category = "Shadowfront|Movement") float SprintSpeed = 720.0f;

    bool bReloading = false;
    bool bSprinting = false;
    bool bDefeated = false;
    float LastDamageTime = -100.0f;
    FVector WeaponRestLocation;
    FVector SpawnLocation;
    FTimerHandle ReloadTimer;
    FTimerHandle RecoilTimer;
    FTimerHandle MuzzleTimer;
    FTimerHandle RespawnTimer;
};
