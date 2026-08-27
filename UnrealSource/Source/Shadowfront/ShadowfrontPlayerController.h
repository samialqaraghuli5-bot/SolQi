#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShadowfrontPlayerController.generated.h"

UCLASS()
class SHADOWFRONT_API AShadowfrontPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    virtual void SetupInputComponent() override;

private:
    void ChooseRecon();
    void ChooseOperation();
    void ChooseVeteran();
    void OnTouchDifficulty(ETouchIndex::Type FingerIndex, FVector Location);
    bool CanChooseDifficulty() const;
};
