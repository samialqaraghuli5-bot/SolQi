#include "ShadowfrontSoldier.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AShadowfrontSoldier::AShadowfrontSoldier()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->BrakingDecelerationWalking = 1900.0f;

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 62.0f));
    FirstPersonCamera->bUsePawnControlRotation = true;
    FirstPersonCamera->FieldOfView = 88.0f;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetupAttachment(FirstPersonCamera);
    WeaponMesh->SetRelativeLocation(FVector(58.0f, 18.0f, -18.0f));
    WeaponMesh->SetRelativeRotation(FRotator(-6.0f, 0.0f, -2.0f));
    WeaponMesh->SetRelativeScale3D(FVector(0.50f, 0.13f, 0.15f));
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    MuzzleLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("MuzzleLight"));
    MuzzleLight->SetupAttachment(WeaponMesh);
    MuzzleLight->SetRelativeLocation(FVector(155.0f, 0.0f, 0.0f));
    MuzzleLight->SetLightColor(FLinearColor(1.0f, 0.34f, 0.08f));
    MuzzleLight->SetIntensity(0.0f);
    MuzzleLight->SetAttenuationRadius(360.0f);
    MuzzleLight->SetCastShadows(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
    if (CubeMesh.Succeeded()) { WeaponMesh->SetStaticMesh(CubeMesh.Object); }
}

void AShadowfrontSoldier::BeginPlay()
{
    Super::BeginPlay();
    Health = MaxHealth;
    MagazineAmmo = MagazineCapacity;
    SpawnLocation = GetActorLocation();
    WeaponRestLocation = WeaponMesh->GetRelativeLocation();
}

void AShadowfrontSoldier::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    WeaponMesh->SetRelativeLocation(FMath::VInterpTo(WeaponMesh->GetRelativeLocation(), WeaponRestLocation, DeltaSeconds, 15.0f));
    if (!bDefeated && Health < MaxHealth && GetWorld()->GetTimeSeconds() > LastDamageTime + 6.0f)
    {
        Health = FMath::Min(MaxHealth, Health + (8.0f * DeltaSeconds));
    }
}

void AShadowfrontSoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShadowfrontSoldier::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShadowfrontSoldier::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShadowfrontSoldier::Fire);
    PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AShadowfrontSoldier::BeginReload);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AShadowfrontSoldier::StartSprint);
    PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AShadowfrontSoldier::StopSprint);
    PlayerInputComponent->BindTouch(IE_Pressed, this, &AShadowfrontSoldier::OnTouchBegin);
}

void AShadowfrontSoldier::MoveForward(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value)) { AddMovementInput(GetActorForwardVector(), Value); }
}

void AShadowfrontSoldier::MoveRight(float Value)
{
    if (Controller && !FMath::IsNearlyZero(Value)) { AddMovementInput(GetActorRightVector(), Value); }
}

void AShadowfrontSoldier::Fire()
{
    if (bDefeated || bReloading) { return; }
    if (MagazineAmmo <= 0) { BeginReload(); return; }
    --MagazineAmmo;

    const FVector TraceStart = FirstPersonCamera->GetComponentLocation();
    const FVector TraceEnd = TraceStart + (FirstPersonCamera->GetForwardVector() * 9000.0f);
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(ShadowfrontShot), false, this);
    GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params);
    if (AActor* HitActor = Hit.GetActor())
    {
        UGameplayStatics::ApplyPointDamage(HitActor, 34.0f, FirstPersonCamera->GetForwardVector(), Hit, GetController(), this, nullptr);
    }

    WeaponMesh->SetRelativeLocation(WeaponRestLocation + FVector(-5.0f, 0.0f, 2.0f));
    MuzzleLight->SetIntensity(3400.0f);
    GetWorldTimerManager().SetTimer(RecoilTimer, this, &AShadowfrontSoldier::FinishRecoil, 0.08f, false);
    GetWorldTimerManager().SetTimer(MuzzleTimer, this, &AShadowfrontSoldier::FinishMuzzleFlash, 0.045f, false);
}

void AShadowfrontSoldier::BeginReload()
{
    if (bDefeated || bReloading || MagazineAmmo >= MagazineCapacity || ReserveAmmo <= 0) { return; }
    bReloading = true;
    StopSprint();
    GetWorldTimerManager().SetTimer(ReloadTimer, this, &AShadowfrontSoldier::FinishReload, 1.25f, false);
}

void AShadowfrontSoldier::FinishReload()
{
    const int32 Loaded = FMath::Min(MagazineCapacity - MagazineAmmo, ReserveAmmo);
    MagazineAmmo += Loaded;
    ReserveAmmo -= Loaded;
    bReloading = false;
}

void AShadowfrontSoldier::FinishRecoil() { WeaponMesh->SetRelativeLocation(WeaponRestLocation); }
void AShadowfrontSoldier::FinishMuzzleFlash() { MuzzleLight->SetIntensity(0.0f); }
void AShadowfrontSoldier::StartSprint()
{
    if (!bReloading && !bDefeated) { bSprinting = true; GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; }
}
void AShadowfrontSoldier::StopSprint() { bSprinting = false; GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; }

void AShadowfrontSoldier::OnTouchBegin(ETouchIndex::Type FingerIndex, FVector Location)
{
    FVector2D ViewportSize(1.0f, 1.0f);
    if (GEngine && GEngine->GameViewport) { GEngine->GameViewport->GetViewportSize(ViewportSize); }
    if (Location.X > ViewportSize.X * 0.68f && Location.Y > ViewportSize.Y * 0.58f) { Fire(); }
    else if (Location.X > ViewportSize.X * 0.76f && Location.Y <= ViewportSize.Y * 0.58f) { BeginReload(); }
    else if (Location.X > ViewportSize.X * 0.54f && Location.Y > ViewportSize.Y * 0.58f) { StartSprint(); }
}

void AShadowfrontSoldier::ApplyCombatDamage(float DamageAmount)
{
    if (bDefeated) { return; }
    LastDamageTime = GetWorld()->GetTimeSeconds();
    Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
    if (Health <= 0.0f)
    {
        bDefeated = true;
        StopSprint();
        DisableInput(Cast<APlayerController>(GetController()));
        GetWorldTimerManager().SetTimer(RespawnTimer, this, &AShadowfrontSoldier::RestoreAfterDefeat, 2.0f, false);
    }
}

void AShadowfrontSoldier::ConfigureDifficulty(float HealthMultiplier)
{
    MaxHealth = BaseMaxHealth * HealthMultiplier;
    Health = MaxHealth;
}

void AShadowfrontSoldier::SetCheckpoint(const FVector& NewCheckpoint) { SpawnLocation = NewCheckpoint; }

void AShadowfrontSoldier::RestoreAfterDefeat()
{
    Health = MaxHealth;
    bDefeated = false;
    SetActorLocation(SpawnLocation);
    EnableInput(Cast<APlayerController>(GetController()));
}

float AShadowfrontSoldier::GetHealthPercent() const { return MaxHealth > 0.0f ? Health / MaxHealth : 0.0f; }
