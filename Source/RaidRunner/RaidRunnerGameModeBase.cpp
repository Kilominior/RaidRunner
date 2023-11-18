// Copyright Epic Games, Inc. All Rights Reserved.


#include "RaidRunnerGameModeBase.h"

void ARaidRunnerGameModeBase::StartPlay()
{
	Super::StartPlay();

	// 显示调试消息五秒
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is RaidRunner's GameMode!"));

}