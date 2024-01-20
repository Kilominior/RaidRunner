// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaidRunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RAIDRUNNER_API ARaidRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
    // 开始游戏时进行必要操作
    virtual void StartPlay() override;

};
