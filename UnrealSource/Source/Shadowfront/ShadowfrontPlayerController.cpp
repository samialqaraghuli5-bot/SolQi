#include "ShadowfrontPlayerController.h"

#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "InputCoreTypes.h"
#include "ShadowfrontGameMode.h"

void AShadowfrontPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    InputComponent->BindKey(EKeys::One, IE_Pressed, this, &AShadowfrontPlayerController::ChooseRecon);
    InputComponent->BindKey(EKeys::Two, IE_Pressed, this, &AShadowfrontPlayerController::ChooseOperation);
    InputComponent->BindKey(EKeys::Three, IE_Pressed, this, &AShadowfrontPlayerController::ChooseVeteran);
    FInputTouchBinding& TouchBinding = InputComponent->BindTouch(IE_Pressed, this, &AShadowfrontPlayerController::OnTouchDifficulty);
    TouchBinding.bConsumeInput = false;
}

bool AShadowfrontPlayerController::CanChooseDifficulty() const
{
    const AShadowfrontGameMode* GameMode = GetWorld() ? Cast<AShadowfrontGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
    return GameMode && GameMode->IsAwaitingDifficulty();
}

void AShadowfrontPlayerController::ChooseRecon()
{
    if (CanChooseDifficulty()) { CastChecked<AShadowfrontGameMode>(GetWorld()->GetAuthGameMode())->SelectDifficulty(EShadowfrontDifficulty::Recon); }
}

void AShadowfrontPlayerController::ChooseOperation()
{
    if (CanChooseDifficulty()) { CastChecked<AShadowfrontGameMode>(GetWorld()->GetAuthGameMode())->SelectDifficulty(EShadowfrontDifficulty::Operation); }
}

void AShadowfrontPlayerController::ChooseVeteran()
{
    if (CanChooseDifficulty()) { CastChecked<AShadowfrontGameMode>(GetWorld()->GetAuthGameMode())->SelectDifficulty(EShadowfrontDifficulty::Veteran); }
}

void AShadowfrontPlayerController::OnTouchDifficulty(ETouchIndex::Type FingerIndex, FVector Location)
{
    if (!CanChooseDifficulty()) { return; }
    FVector2D ViewportSize(1.0f, 1.0f);
    if (GEngine && GEngine->GameViewport) { GEngine->GameViewport->GetViewportSize(ViewportSize); }
    if (Location.X < ViewportSize.X / 3.0f) { ChooseRecon(); }
    else if (Location.X < ViewportSize.X * (2.0f / 3.0f)) { ChooseOperation(); }
    else { ChooseVeteran(); }
}
