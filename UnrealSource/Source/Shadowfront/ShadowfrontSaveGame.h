#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ShadowfrontSaveGame.generated.h"

UCLASS()
class SHADOWFRONT_API UShadowfrontSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY() int32 CurrentWave = 0;
    UPROPERTY() FString DifficultyName = TEXT("OPERATION");
    UPROPERTY() bool bMissionComplete = false;
};
