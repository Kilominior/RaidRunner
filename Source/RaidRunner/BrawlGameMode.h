// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BrawlGameMode.generated.h"

// 组播角色死亡信息
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerDiedSignature, ACharacter*, Character, AController*, CharacterController);

/**
 * 乱斗模式
 */
UCLASS()
class RAIDRUNNER_API ABrawlGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

    // 尝试重新生成玩家的Pawn
    virtual void RestartPlayer(AController* NewPlayer) override;

protected:
    // 开始游戏时进行必要操作
    virtual void BeginPlay() override;

    // 玩家死亡时调用
    UFUNCTION()
    virtual void PlayerDied(ACharacter* Character, AController* CharacterController);

    // 绑定死亡委托的签名
    UPROPERTY()
    FOnPlayerDiedSignature OnPlayerDied;
};
