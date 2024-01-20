// Fill out your copyright notice in the Description page of Project Settings.


#include "BrawlGameMode.h"
#include "GameFramework/Character.h"

void ABrawlGameMode::RestartPlayer(AController* NewPlayer)
{
    Super::RestartPlayer(NewPlayer);
}

void ABrawlGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 显示调试消息五秒
    check(GEngine != nullptr);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello, this is Brawl GameMode!"));

    UWorld* PersistentWorld = GetWorld();
    FString LevelName = PersistentWorld->GetCurrentLevel()->GetPathName();
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, LevelName);

    // 将玩家死亡的委托绑定到对应的处理函数
    if (!OnPlayerDied.IsBound())
    {
        OnPlayerDied.AddDynamic(this, &ABrawlGameMode::PlayerDied);
    }

}

void ABrawlGameMode::PlayerDied(ACharacter* Character, AController* CharacterController)
{
    // 将角色加入复活栈

    // 开始复活倒计时

    FString RespawnMessage = FString::Printf(TEXT("正在复活玩家%s"), *Character->GetFName().ToString());
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, RespawnMessage);

    // 传入死亡玩家的Controller执行复活
    RestartPlayer(CharacterController);
}
