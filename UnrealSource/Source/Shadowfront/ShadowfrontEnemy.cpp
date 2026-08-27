#include "ShadowfrontEnemy.h"

#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShadowfrontGameMode.h"
#include "ShadowfrontSoldier.h"
#include "TimerManager.h"

AShadowfrontEnemy::AShadowfrontEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->MaxWalkSpeed = 150.0f;
    GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
    GetCapsuleComponent()->SetCapsuleRadius(38.0f);

    TorsoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TorsoMesh"));
    TorsoMesh->SetupAttachment(GetCapsuleComponent());
    TorsoMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));
    TorsoMesh->SetRelativeScale3D(FVector(0.55f, 0.36f, 1.30f));
    TorsoMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    HeadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadMesh"));
    HeadMesh->SetupAttachment(GetCapsuleComponent());
    HeadMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 86.0f));
    HeadMesh->SetRelativeScale3D(FVector(0.32f));
    HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MarkerLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MarkerLight"));
    MarkerLight->SetupAttachment(GetCapsuleComponent());
    MarkerLight->SetRelativeLocation(FVector(0.0f, 0.0f, 58.0f));
    MarkerLight->SetIntensity(420.0f);
    MarkerLight->SetAttenuationRadius(340.0f);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded()) { TorsoMesh->SetStaticMesh(CubeMesh.Object); HeadMesh->SetStaticMesh(CubeMesh.Object); }
}

void AShadowfrontEnemy::ConfigureEnemy(EShadowfrontEnemyRole InRole, float InDifficultyMultiplier)
{
    EnemyRole = InRole;
    Health = BaseHealth * InDifficultyMultiplier;
    DamagePerShot = 7.0f * InDifficultyMultiplier;
    AttackRange = 2300.0f;
    PreferredRange = 1500.0f;
    FireInterval = 1.35f;
    AdvanceSpeed = 115.0f;

    switch (EnemyRole)
    {
        case EShadowfrontEnemyRole::Breacher:
            Health *= 1.15f; DamagePerShot *= 1.15f; AttackRange = 1250.0f; PreferredRange = 750.0f; FireInterval = 0.95f; AdvanceSpeed = 205.0f; break;
        case EShadowfrontEnemyRole::Spotter:
            Health *= 0.75f; DamagePerShot *= 0.80f; AttackRange = 3100.0f; PreferredRange = 2500.0f; FireInterval = 1.85f; AdvanceSpeed = 65.0f; break;
        case EShadowfrontEnemyRole::DroneOperator:
            Health *= 0.90f; DamagePerShot *= 1.05f; AttackRange = 2600.0f; PreferredRange = 1850.0f; FireInterval = 1.10f; AdvanceSpeed = 95.0f; break;
        default: break;
    }
    ApplyRoleVisuals();
}

void AShadowfrontEnemy::BeginPlay()
{
    Super::BeginPlay();
    if (Health <= 0.0f) { Health = BaseHealth; }
    ApplyRoleVisuals();
    GetWorldTimerManager().SetTimer(FireTimer, this, &AShadowfrontEnemy::TryFire, FireInterval, true, 0.8f);
}

void AShadowfrontEnemy::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bNeutralized) { return; }
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!PlayerPawn) { return; }
    const FVector ToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    const float Distance = ToPlayer.Size2D();
    if (Distance > AttackRange || !HasClearShot(PlayerPawn)) { return; }

    const FVector FlatTarget(PlayerPawn->GetActorLocation().X, PlayerPawn->GetActorLocation().Y, GetActorLocation().Z);
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), (FlatTarget - GetActorLocation()).Rotation(), DeltaSeconds, 2.35f));
    if (Distance > PreferredRange)
    {
        const FVector Step = ToPlayer.GetSafeNormal2D() * AdvanceSpeed * DeltaSeconds;
        AddActorWorldOffset(Step, true);
    }
}

void AShadowfrontEnemy::TryFire()
{
    if (bNeutralized) { return; }
    if (AShadowfrontSoldier* Player = Cast<AShadowfrontSoldier>(UGameplayStatics::GetPlayerCharacter(this, 0)))
    {
        if (FVector::Distance(GetActorLocation(), Player->GetActorLocation()) < AttackRange && HasClearShot(Player))
        {
            Player->ApplyCombatDamage(DamagePerShot);
        }
    }
}

bool AShadowfrontEnemy::HasClearShot(const APawn* PlayerPawn) const
{
    if (!PlayerPawn || !GetWorld()) { return false; }
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(ShadowfrontEnemySight), false, this);
    Params.AddIgnoredActor(PlayerPawn);
    const FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 62.0f);
    const FVector End = PlayerPawn->GetActorLocation() + FVector(0.0f, 0.0f, 54.0f);
    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
    return !bHit;
}

float AShadowfrontEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float AppliedDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (!bNeutralized && DamageAmount > 0.0f)
    {
        Health -= DamageAmount;
        if (Health <= 0.0f) { Neutralize(); }
    }
    return AppliedDamage;
}

void AShadowfrontEnemy::ApplyRoleVisuals()
{
    if (!MarkerLight) { return; }
    FLinearColor MarkerColor(1.0f, 0.23f, 0.06f);
    switch (EnemyRole)
    {
        case EShadowfrontEnemyRole::Breacher: MarkerColor = FLinearColor(1.0f, 0.48f, 0.08f); break;
        case EShadowfrontEnemyRole::Spotter: MarkerColor = FLinearColor(0.35f, 0.62f, 1.0f); break;
        case EShadowfrontEnemyRole::DroneOperator: MarkerColor = FLinearColor(0.78f, 0.24f, 1.0f); break;
        default: break;
    }
    MarkerLight->SetLightColor(MarkerColor);
}

void AShadowfrontEnemy::Neutralize()
{
    bNeutralized = true;
    GetWorldTimerManager().ClearTimer(FireTimer);
    MarkerLight->SetLightColor(FLinearColor(0.12f, 0.90f, 0.52f));
    SetActorEnableCollision(false);
    if (AShadowfrontGameMode* Mode = Cast<AShadowfrontGameMode>(UGameplayStatics::GetGameMode(this))) { Mode->NotifyEnemyEliminated(); }
    SetLifeSpan(4.0f);
}
